#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "digitalCloset_Motorcontrol.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void onMouse1(int event,int x,int y,int flags,void* param);                                 
void onMouse2(int event,int x,int y,int flags,void* param);


static void help()
{
    cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
            "This classifier can recognize many ~rigid objects, it's most known use is for faces.\n"
            "Usage:\n"
            "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
               "   [--nested-cascade[=nested_cascade_path this an optional secondary classifier such as eyes]]\n"
               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>\n"
               "   [filename|camera_index]\n\n"
            "see facedetect.cmd for one call:\n"
            "./facedetect --cascade=\"../../data/haarcascades/haarcascade_frontalface_alt.xml\" --nested-cascade=\"../../data/haarcascades/haarcascade_eye.xml\" --scale=1.3 \n"
            "Hit any key to quit.\n"
            "Using OpenCV version " << CV_VERSION << "\n" << endl;
}

void detectAndDraw( Mat& img,
                   CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
                   double scale);


/*===================================global var================================== */

/*cascade string*/
String cascadeName = "../../data/haarcascades/haarcascade_frontalface_alt.xml";
String nestedCascadeName = "../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

/*Load image file used*/
IplImage *cloth_img_select = cvCreateImage(cvSize(150,150), IPL_DEPTH_8U, 3);
IplImage *cloth_img1 = NULL;
IplImage *cloth_img2 = NULL;
IplImage *cloth_img3 = NULL;

/*closet file name string*/
char cloth1[]= {"cloth1.jpg"};
char cloth2[]= {"cloth3.bmp"};
char cloth3[]= {"cloth3.bmp"};

/*switch on off copy cloth img*/
int switch_cloth_trigger = 0;

int frame_index=0;
/*================================================================================ */


int main( int argc, const char** argv )
{
    CvCapture* capture = 0;
    Mat frame, frameCopy, image;
    const String scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const String cascadeOpt = "--cascade=";
    size_t cascadeOptLen = cascadeOpt.length();
    const String nestedCascadeOpt = "--nested-cascade";
    size_t nestedCascadeOptLen = nestedCascadeOpt.length();
    String inputName;

    help();
    motor_init();

    CascadeClassifier cascade, nestedCascade;
    double scale = 1;

	/*construct closets*/
    cvNamedWindow( "closet1", 1 );
    cvNamedWindow( "closet2", 1 );
    cvNamedWindow( "closet3", 1 );

    /*mouse control*/                                                                       
    cvSetMouseCallback("closet1",onMouse1,NULL);
    cvSetMouseCallback("closet2",onMouse2,NULL);

	/*load cloth image*/
	cloth_img1 = cvLoadImage(cloth1,1);
	cloth_img2 = cvLoadImage(cloth2,1);
	cloth_img3 = cvLoadImage(cloth3,1);
	if(!cloth_img1 || !cloth_img2 || !cloth_img3) {
			printf("No such image file\n");                
			return 0;
	}
	else{
			/*show cloth img*/
			cvShowImage("closet1",cloth_img1);
			cvShowImage("closet2",cloth_img2);
	}


    for( int i = 1; i < argc; i++ )
    {
        cout << "Processing " << i << " " <<  argv[i] << endl;
        if( cascadeOpt.compare( 0, cascadeOptLen, argv[i], cascadeOptLen ) == 0 )
        {
            cascadeName.assign( argv[i] + cascadeOptLen );
            cout << "  from which we have cascadeName= " << cascadeName << endl;
        }
        else if( nestedCascadeOpt.compare( 0, nestedCascadeOptLen, argv[i], nestedCascadeOptLen ) == 0 )
        {
            if( argv[i][nestedCascadeOpt.length()] == '=' )
                nestedCascadeName.assign( argv[i] + nestedCascadeOpt.length() + 1 );
            if( !nestedCascade.load( nestedCascadeName ) )
                cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
        }
        else if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 )
        {
            if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
                scale = 1;
            cout << " from which we read scale = " << scale << endl;
        }
        else if( argv[i][0] == '-' )
        {
            cerr << "WARNING: Unknown option %s" << argv[i] << endl;
        }
        else
            inputName.assign( argv[i] );
    }

    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=<cascade_path>]\n"
            "   [--nested-cascade[=nested_cascade_path]]\n"
            "   [--scale[=<image scale>\n"
            "   [filename|camera_index]\n" << endl ;
        return -1;
    }

    if( inputName.empty() || (isdigit(inputName.c_str()[0]) && inputName.c_str()[1] == '\0') )
    {
        capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
		/*set camera resolution*/
		cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,320);                       
        cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,240); 
        int c = inputName.empty() ? 0 : inputName.c_str()[0] - '0' ;
        if(!capture) cout << "Capture from CAM " <<  c << " didn't work" << endl;
    }
    else if( inputName.size() )
    {
        image = imread( inputName, 1 );
        if( image.empty() )
        {
            capture = cvCaptureFromAVI( inputName.c_str() );
            if(!capture) cout << "Capture from AVI didn't work" << endl;
        }
    }
    else
    {
        image = imread( "lena.jpg", 1 );
        if(image.empty()) cout << "Couldn't read lena.jpg" << endl;
    }

    cvNamedWindow( "result", 1 );

    if( capture )
    {
        cout << "In capture ..." << endl;

        #if 0
        for(;;)
        {
            IplImage* iplImg = cvQueryFrame( capture );
            frame = iplImg;
            if( frame.empty() )
                break;
            if( iplImg->origin == IPL_ORIGIN_TL )
                frame.copyTo( frameCopy );
            else
                flip( frame, frameCopy, 0 );

            detectAndDraw( frameCopy, cascade, nestedCascade, scale );

            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }
        #endif

            for(frame_index=0 ;;frame_index++)
                {
                    IplImage* iplImg = cvQueryFrame( capture );
                    frame = iplImg;
                    printf("%d %d \n" , frame.cols,frame.rows);
                    if( frame.empty() )
                                    break;                                                                                                                                         


                    /*in our project , we capture 100 frame , but only detect face 1 frame */
                    if(switch_cloth_trigger == 1){ 
            //if(1){ 
                        if( iplImg->origin == IPL_ORIGIN_TL )
                            frame.copyTo( frameCopy );
                        else
                            flip( frame, frameCopy, 0 );

                        detectAndDraw( frameCopy, cascade, nestedCascade, scale );
                    }
                    else{
                        cv::imshow( "result", frame );
                    }
                    if( waitKey( 10 ) >= 0 )
                        goto _cleanup_;
                }

        waitKey(0);

_cleanup_:
        cvReleaseCapture( &capture );
    }
    else
    {
        cout << "In image read" << endl;
        if( !image.empty() )
        {
            detectAndDraw( image, cascade, nestedCascade, scale );
            waitKey(0);
        }
        else if( !inputName.empty() )
        {
            /* assume it is a text file containing the
            list of the image filenames to be processed - one per line */
            FILE* f = fopen( inputName.c_str(), "rt" );
            if( f )
            {
                char buf[1000+1];
                while( fgets( buf, 1000, f ) )
                {
                    int len = (int)strlen(buf), c;
                    while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';
                    cout << "file " << buf << endl;
                    image = imread( buf, 1 );
                    if( !image.empty() )
                    {
                        detectAndDraw( image, cascade, nestedCascade, scale );
                        c = waitKey(0);
                        if( c == 27 || c == 'q' || c == 'Q' )
                            break;
                    }
                    else
                    {
                            cerr << "Aw snap, couldn't read image " << buf << endl;
                    }
                }
                fclose(f);
            }
        }
    }
    cvDestroyWindow("result");
    cvDestroyWindow("closet1");
    cvDestroyWindow("closet2");
    cvDestroyWindow("closet3");
    cvReleaseImage(&cloth_img1);
    cvReleaseImage(&cloth_img2);
    cvReleaseImage(&cloth_img3);
    return 0;
}

void detectAndDraw( Mat& img,
                   CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
                   double scale)
{
    int i = 0;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,255,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius,rec_width,rec_height,i,j,k;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        /*Draw circle around face*/
                 //circle( img, center, radius, color, 3, 8, 0 );             
        /*Rectangle*/
        CvPoint point1, point2;                
        point1.x = r->x - r->width*0.9;
        point2.x = r->x + r->width*1.8;
        point1.y = r->y + r->height*0.9;
        point2.y = r->y + r->height*4.1;
        //calculate rect. width&height
        rec_width = point2.x-point1.x;
        rec_height = point2.y-point1.y;
        printf("rec_height=%d , rec_width=%d\n" , rec_height, rec_width);
        printf("Point1 is (%d,%d)\n",point1.x,point1.y);
        printf("Point2 is (%d,%d)\n",point2.x,point2.y);

        //point1.y = 125;
        //point1.x = 125;

        /*Put on clothes function*/
		//if(switch_cloth_trigger){
		for(i = point1.y ; i < point1.y+150 ; i++){
			if (i<480)
			{
				for(j = (point1.x)*3  ; j < (150+point1.x)*3 ; j = j+3){       /*if(signed char "-1" >> white[255])*/

					if( (cloth_img_select->imageData[(i-point1.y)*452+j-point1.x*3] + cloth_img_select->imageData[(i-point1.y)*452+j-point1.x*3+1] + cloth_img_select->imageData[(i-point1.y)*452+j-point1.x*3+2])/3 >200 ){/*do not copy from cloth_image*/
					}
					else{
						img.data[i*( img.cols * 3)+j] = cloth_img_select->imageData[(i-point1.y)*452+j-point1.x*3];            
						img.data[i*( img.cols * 3)+j+1] = cloth_img_select->imageData[(i-point1.y)*452+j-point1.x*3+1];
						img.data[i*( img.cols * 3)+j+2] = cloth_img_select->imageData[(i-point1.y)*452+j-point1.x*3+2];
					}
				}	
    		}
		}
		//}
    	#if 0
        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg(*r);
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CV_HAAR_FIND_BIGGEST_OBJECT
            //|CV_HAAR_DO_ROUGH_SEARCH
            //|CV_HAAR_DO_CANNY_PRUNING
            |CV_HAAR_SCALE_IMAGE
            ,
            Size(30, 30) );
        for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
        {
            center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
            center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
            radius = cvRound((nr->width + nr->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        #endif                                            
    }
    cv::imshow( "result", img );
}


void onMouse1(int event,int x,int y,int flag,void* param){

        /*mouse left button down*/
        if(event==CV_EVENT_LBUTTONDOWN){
				cloth_img_select->imageData = cloth_img1->imageData;
				//memcpy(cloth_img_select->imageData , cloth_img )
                switch_cloth_trigger = 1;
        }
        /*mouse right button down*/
        if(event==CV_EVENT_RBUTTONDOWN){
            motor_control(8, 1, 1);
            sleep(5);
            motor_control(8, 1, 2);
        }
        if(flag==CV_EVENT_FLAG_LBUTTON){
        }
        if(event==CV_EVENT_MOUSEMOVE){
        }
}
void onMouse2(int event,int x,int y,int flag,void* param){

        /*mouse left button down*/
        if(event==CV_EVENT_LBUTTONDOWN){
                cloth_img_select->imageData = cloth_img2->imageData;
                switch_cloth_trigger = 1;
        }
        /*mouse right button down*/
        if(event==CV_EVENT_RBUTTONDOWN){
            motor_control(8, 2, 1);
            sleep(5);
            motor_control(8, 2, 2);
        }
        if(flag==CV_EVENT_FLAG_LBUTTON){
        }
        if(event==CV_EVENT_MOUSEMOVE){
        }
}


