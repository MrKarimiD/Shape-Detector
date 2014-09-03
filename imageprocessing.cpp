#include "imageprocessing.h"

ImageProcessing::ImageProcessing(QObject *parent) :
    QObject(parent)
{
    drawBoundedRect=false;
    drawContoursBool=false;
    drawGeometricLabels=false;
    drawRotatedRect=false;
    drawBoundries=false;
    filterSetting=new filterSettings();
    //sendingSocket = new NetworkSender();

    //connect(this,SIGNAL(gameGroundReady(GameGround)),this,SLOT(convert(GameGround)));
}

Mat ImageProcessing::shapeDetection(Mat input, Mat src, Rect cropedRect)
{
    imSize.width = input.cols;
    imSize.height = input.rows;

    result.clear_shapes();
    result.set_numberofshape(0);

    // Find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(input.clone(), contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

    vector<Point> approx;
    Mat dst = src.clone();
    RNG rng(12345);
    if(drawBoundries)
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

        if(this->drawContoursBool)
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(dst,contours, i, color, 2, 8, hierarchy, 0, Point(cropedRect.x,cropedRect.y) );
        }

        if(drawBoundedRect)
        {
            Rect boundedRect=boundingRect( Mat(contours[i]) );
            rectangle( dst, boundedRect.tl()+cropedRect.tl(), boundedRect.br()+cropedRect.tl(), Scalar(0,128,255), 1, 8, 0 );
        }

        if(drawRotatedRect)
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
            prepareDataForOutput(contours[i],"TRI");
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
                prepareDataForOutput(contours[i],"RECT");
            }
            else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
            {
                setLabel(dst, "PENTA", contours[i],cropedRect);
                prepareDataForOutput(contours[i],"PENTA");
            }
            else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
            {
                    setLabel(dst, "HEXA", contours[i],cropedRect);
                    prepareDataForOutput(contours[i],"HEXA");
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
                prepareDataForOutput(contours[i],"CIR");
            }
        }
    }

   dst.copyTo(Outputs[4]);
   return dst;
}

QString ImageProcessing::returnHsv(Mat input)
{
    Mat Hsv,img;
    cvtColor(input,Hsv,COLOR_RGB2HSV);
    imshow("hsv",Hsv);
    //inRange(Hsv, Scalar(0, 0, 0), Scalar(38, 255, 255), img);
    //imshow("img",img);
    Scalar ff= mean(Hsv);
    qDebug()<<"mean:"<<QString::number(ff.val[0]);
    qDebug()<<"mean:"<<QString::number(ff.val[1]);
    qDebug()<<"mean:"<<QString::number(ff.val[2]);
}

Mat ImageProcessing::applyFilters(Mat input)
{
    Mat src = input.clone();

    // Convert to grayscale
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    if(filterSetting->getUseMedianBlur())
    {
        medianBlur(gray,gray,filterSetting->getKernelSize());
    }

    Mat threshold_mat = Mat::zeros(gray.cols,gray.rows,CV_8UC1);
    if(filterSetting->getUseAdaptiveThresh())
    {
        adaptiveThreshold(gray,threshold_mat,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY_INV,
        filterSetting->getBlockSize(),
        filterSetting->getC());
        threshold_mat.copyTo(Outputs[1]);
    }
    else
    {
        if(filterSetting->getUseThreshold())
        {
            threshold( gray, threshold_mat,filterSetting->getThreshValue(),255,THRESH_BINARY_INV);
            threshold_mat.copyTo(Outputs[2]);
        }
        else
        {
            threshold_mat=gray.clone();
        }
    }

    if(filterSetting->getUseDilate())
    {
        Mat structure=getStructuringElement(MORPH_RECT,Size(filterSetting->getDilateSize(),filterSetting->getDilateSize()));
        dilate(threshold_mat,threshold_mat,structure);
    }

    // Use Canny instead of threshold to catch squares with gradient shading
    Mat bw;
    if(filterSetting->getUseCanny())
    {
        Canny(threshold_mat, bw,filterSetting->getFirstThresh(),filterSetting->getSecondThresh()
        , filterSetting->getApertureSize());
        bw.copyTo(Outputs[3]);
    }
    else
    {
        bw=threshold_mat.clone();
    }

    return bw;
}

Mat ImageProcessing::undistortImage(Mat input)
{
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

    cameraMatrix.at<double>(0,0) = 6.7055726712006776e+02;
    cameraMatrix.at<double>(0,1) = 0;
    cameraMatrix.at<double>(0,2) = 3.8950000000000000e+02;

    cameraMatrix.at<double>(1,0) = 0;
    cameraMatrix.at<double>(1,1) = 6.7055726712006776e+02;
    cameraMatrix.at<double>(1,2) = 2.8950000000000000e+02;

    cameraMatrix.at<double>(2,0) = 0;
    cameraMatrix.at<double>(2,1) = 0;
    cameraMatrix.at<double>(2,2) = 1;

    Mat distCoeffs = Mat::zeros(8, 1, CV_64F);

    distCoeffs.at<double>(0,0) = -3.7087470577837894e-01;
    distCoeffs.at<double>(1,0) = 1.8508542088322785e-01;
    distCoeffs.at<double>(2,0) = 0;
    distCoeffs.at<double>(3,0) = 0;
    distCoeffs.at<double>(4,0) = -3.4799226907590207e-02;

    Mat inputFrame;
    input.copyTo(inputFrame);

//    bitwise_not(inputFrame, inputFrame);

    Mat outputFrame;
//    undistort(input,outputFrame,cameraMatrix,distCoeffs);
    Size imageSize = input.size();
    Mat view, rview, map1, map2;

    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
        getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 0, imageSize, 0),
        imageSize, CV_16SC2, map1, map2);
    view = inputFrame;
    remap(view, rview, map1, map2, INTER_LINEAR);

//    //blur(outputFrame,outputFrame,Size(3,3));
//    //imshow("dis",input);
////    imshow("undis",rview);
    return rview;
}

double ImageProcessing::angle(Point pt1, Point pt2, Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

void ImageProcessing::setLabel(Mat &im, const string label, std::vector<Point> &contour,Rect cropedRect)
{
    if(this->drawGeometricLabels)
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

void ImageProcessing::changeOutputSetting(bool con, bool geom, bool bound, bool rotate,bool boundries)
{
    this->drawContoursBool=con;
    this->drawGeometricLabels=geom;
    this->drawBoundedRect=bound;
    this->drawRotatedRect=rotate;
    this->drawBoundries=boundries;
}

void ImageProcessing::updateFilterSettings(filterSettings *fs)
{
    this->filterSetting=fs;
}

Mat ImageProcessing::returnAdaptiveThreshlodImage()
{
    return Outputs[1];
}

Mat ImageProcessing::returnThreshlodImage()
{
    return Outputs[2];
}

Mat ImageProcessing::returnCannyImage()
{
    return Outputs[3];
}

Mat ImageProcessing::returnCropedImage()
{
    return Outputs[0];
}

void ImageProcessing::addShape(float x, float y, double radius, string type, string color)
{
    outputPacket_shape *shape=result.add_shapes();
    shape->set_radios(radius);
    shape->set_type(type);
    shape->set_color(color);
    shape->set_position_x(x);
    shape->set_position_y(y);
}

void ImageProcessing::findColors(Mat input)
{
    Mat src,thresh;
//    input.copyTo(src);

    cvtColor(input,src,COLOR_BGR2HSV);
    inRange(src, Scalar(90,150,150), Scalar(170,255,255), thresh);
}

bool ImageProcessing::checkAspectRatio(vector<Point> contours_poly)
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

bool ImageProcessing::checkAspectRatioForRotatedRect(RotatedRect input)
{
    double aspect_ratio = float(input.size.width)/input.size.height;
    bool out=true;

    if(  (aspect_ratio>ASPECT_RATIO_TRESH) || (aspect_ratio<(1/ASPECT_RATIO_TRESH)))
        out=false;

    return out;
}

void ImageProcessing::prepareDataForOutput(std::vector<Point> &contour, QString type)
{
    result.set_numberofshape(result.numberofshape()+1);
    Point2f center;
    float radius;
    minEnclosingCircle( (Mat)contour, center, radius );
    //--------mohsen khare---------------
    float Xman = Orgin_X - (center.x/imSize.width)*Width;
    float Yman = Orgin_Y + (center.y/imSize.height)*Height;
    //---------------------------------
    addShape(Xman+100,Yman-100,radius,type.toStdString(),"UnCheck");
}
