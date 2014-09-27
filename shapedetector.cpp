#include "shapedetector.h"

shapeDetector::shapeDetector(QObject *parent) :
    ImageProcFunction(parent)
{
    colorDetector = new ColorDetector();
    os = new showOutputSetting();
    semaphoreForUpdateShowSettings = new QSemaphore(1);
    //semaphoreForAddingSource = new QSemaphore(1);
    semaphoreForUpdateFilterSetting = new QSemaphore(1);
}

void shapeDetector::process()
{
    disconnect(timer,SIGNAL(timeout()),this,SLOT(timer_interval()));

    semaphore->tryAcquire(1,30);
    Mat filter = input.canny;
    Mat destination = input.final;
    this->newUnreadData = false;
    semaphore->release(1);

    filterSettings *filterSet = new filterSettings();
    semaphoreForUpdateFilterSetting->tryAcquire(1,30);
    filterSet = fs;
    semaphoreForUpdateFilterSetting->release(1);

    Rect cropedRect;
    if(filterSet->getUseCrop())
    {

        cropedRect.x = filterSet->getCropFirstX();
        cropedRect.y = filterSet->getCropFirstY();
        cropedRect.width = filterSet->getCropSecondX() - filterSet->getCropFirstX();
        cropedRect.height = filterSet->getCropSecondY() - filterSet->getCropFirstY();
    }
    else
    {
        cropedRect.x = 0;
        cropedRect.y = 0;
        cropedRect.width = filter.rows;
        cropedRect.height = filter.cols;
    }

    Mat out = shapeDetection(filter,destination,cropedRect);

    semaphore->tryAcquire(1,30);
    MatArray output = this->input;
    output.final = out;
    semaphore->release(1);

    connect(timer,SIGNAL(timeout()),this,SLOT(timer_interval()));

    emit done(output);
}

void shapeDetector::updateShowSetting(showOutputSetting *os)
{
    semaphoreForUpdateShowSettings->acquire(1);
    this->os = os;
    semaphoreForUpdateShowSettings->release(1);
}

void shapeDetector::updateFilterSetting(filterSettings *fs)
{
    semaphoreForUpdateFilterSetting->acquire(1);
    this->fs = fs;
    semaphoreForUpdateFilterSetting->release(1);
}

void shapeDetector::addShape(float x, float y, double radius, string type, string color)
{
    outputPacket_shape *shape=result.add_shapes();
    shape->set_radios(radius);
    shape->set_type(type);
    shape->set_color(color);
    shape->set_position_x(x);
    shape->set_position_y(y);
}

Mat shapeDetector::shapeDetection(Mat filter, Mat destination, Rect cropedRect)
{
    semaphoreForUpdateShowSettings->acquire(1);
    showOutputSetting *outSet = new showOutputSetting();
    outSet = os;
    semaphoreForUpdateShowSettings->release(1);

    imSize.width = filter.cols;
    imSize.height = filter.rows;

    result.clear_shapes();
    result.set_numberofshape(0);

    // Find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(filter.clone(), contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

    vector<Point> approx;
    Mat dst = destination.clone();
    RNG rng(12345);
    if(outSet->get_drawBoundries())
    {
        rectangle(dst,cropedRect,Scalar(0,0,0));
    }

    for (int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional
        // to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

        // Skip small or non-convex objects
        if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
            continue;

        if(!checkAspectRatio(contours[i]))
            continue;

        RotatedRect rotatetBoundRect=minAreaRect(Mat(contours[i]));
        if(!checkAspectRatioForRotatedRect(rotatetBoundRect))
        {
            continue;
        }

        if(outSet->get_drawContoursBool())
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(dst,contours, i, color, 2, 8, hierarchy, 0, Point(cropedRect.x,cropedRect.y) );
        }

        if(outSet->get_drawBoundedRect())
        {
            Rect boundedRect=boundingRect( Mat(contours[i]) );
            rectangle( dst, boundedRect.tl()+cropedRect.tl(), boundedRect.br()+cropedRect.tl(), Scalar(0,128,255), 1, 8, 0 );
        }

        if(outSet->get_drawRotatedRect())
        {
            Point2f vertices[4];
            rotatetBoundRect.points(vertices);
            for (int i = 0; i < 4; i++)
            {
                vertices[i].x+=cropedRect.tl().x;
                vertices[i].y+=cropedRect.tl().y;
            }

            for (int i = 0; i < 4; i++)
            {
                line(dst, vertices[i], vertices[(i+1)%4], Scalar(147,20,255),2);
            }
        }

        if (approx.size() == 3)
        {
            setLabel(dst, "TRI", contours[i],cropedRect);    // Triangles
            prepareDataForOutput(contours[i],"TRI",destination,cropedRect);
        }
        else if (approx.size() >= 4 && approx.size() <= 6)
        {
            // Number of vertices of polygonal curve
            int vtc = approx.size();

            // Get the cosines of all corners
            vector<double> cos;
            for (int j = 2; j < vtc+1; j++)
                cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

            // Sort ascending the cosine values
            sort(cos.begin(), cos.end());

            // Get the lowest and the highest cosine
            double mincos = cos.front();
            double maxcos = cos.back();

            // Use the degrees obtained above and the number of vertices
            // to determine the shape of the contour
            if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
            {
                setLabel(dst, "RECT", contours[i],cropedRect);
                //imshow("in",src);
                prepareDataForOutput(contours[i],"RECT",destination,cropedRect);
            }
            else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
            {
                setLabel(dst, "PENTA", contours[i],cropedRect);
                prepareDataForOutput(contours[i],"PENTA",destination,cropedRect);
            }
            else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
            {
                setLabel(dst, "HEXA", contours[i],cropedRect);
                prepareDataForOutput(contours[i],"HEXA",destination,cropedRect);
            }
        }
        else
        {
            // Detect and label circles
            double area = contourArea(contours[i]);
            Rect r = boundingRect(contours[i]);

            int radius = r.width / 2;

            if (abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                    abs(1 - (area / (CV_PI * pow(radius, 2)))) <= 0.2)
            {
                setLabel(dst, "CIR", contours[i],cropedRect);
                prepareDataForOutput(contours[i],"CIR",destination,cropedRect);
            }
        }
    }

    //destination.copyTo(Outputs[4]);

    return dst;
}

bool shapeDetector::checkAspectRatio(vector<Point> contours_poly)
{
    Rect boundedRect=boundingRect( Mat(contours_poly) );
    double aspect_ratio = float(boundedRect.width)/boundedRect.height;
    bool out;

    if(  (aspect_ratio>ASPECT_RATIO_TRESH) || (aspect_ratio<(1/ASPECT_RATIO_TRESH)))
        out=false;
    else
        out=true;

    return out;
}

bool shapeDetector::checkAspectRatioForRotatedRect(RotatedRect input)
{
    double aspect_ratio = float(input.size.width)/input.size.height;
    bool out=true;

    if(  (aspect_ratio>ASPECT_RATIO_TRESH) || (aspect_ratio<(1/ASPECT_RATIO_TRESH)))
        out=false;

    return out;
}

double shapeDetector::angle(Point pt1, Point pt2, Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void shapeDetector::setLabel(Mat &im, const string label, std::vector<Point> &contour, Rect cropedRect)
{
    if(os->get_drawGeometricLabels())
    {
        int fontface = FONT_HERSHEY_SIMPLEX;
        double scale = 0.4;
        int thickness = 1;
        int baseline = 0;

        Size text = getTextSize(label, fontface, scale, thickness, &baseline);
        Rect r = boundingRect(contour);

        Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
        pt.x+=cropedRect.x;
        pt.y+=cropedRect.y;
        rectangle(im, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
        putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
    }
}

void shapeDetector::prepareDataForOutput(std::vector<Point> &contour, QString type, Mat frame, Rect crop)
{
    result.set_numberofshape(result.numberofshape()+1);
    Point2f center;
    float radius;
    minEnclosingCircle( (Mat)contour, center, radius );

    Point2f realCenter;
    realCenter.x = center.x + crop.tl().x;
    realCenter.y = center.y + crop.tl().y;

    Rect sample_crop;
    sample_crop.x = realCenter.x-10;
    sample_crop.y = realCenter.y-10;
    sample_crop.height = 20;
    sample_crop.width = 20;

    Mat sample_frame(frame,sample_crop);

    Mat HSV;
    cvtColor(sample_frame,HSV,COLOR_RGB2HSV);

    vector<Mat> hsvPlanes;
    split(HSV, hsvPlanes);

    //--------mohsen khare---------------
    float Xman = Orgin_X - (center.x/imSize.width)*Width;
    float Yman = Orgin_Y + (center.y/imSize.height)*Height;
    //---------------------------------
    addShape(Xman+100,Yman-100,radius,type.toStdString(),colorDetector->findColor(hsvPlanes).toStdString());
}
