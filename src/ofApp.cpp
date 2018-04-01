#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    /*
     
     
     
     ＝＝＝＝＝＝＝＝＝＝＝ＰＯＲＴＲＡＩＴ＝＝＝＝＝＝＝＝＝＝＝
     
     
     
     */
    
    image.load("img.jpg");
    
    dst = image.getPixels();
    
    imgWarp.setup(dst.getWidth(), dst.getHeight(), 50);
    
    tex.allocate(dst);
    tex.loadData(dst);
    
    ofxXmlSettings settings;
    
    if(settings.loadFile("settings.xml")){
        settings.pushTag("settings");
        int numberOfSavedPoints = settings.getNumTags("position");
        cout<<numberOfSavedPoints<<endl;
        for(int i = 0; i < numberOfSavedPoints; i++){
            settings.pushTag("position", i);
            
            ofPoint p;
            p.x = settings.getValue("x",0);
            p.y = settings.getValue("y",0);
            ctrl.push_back(make_pair(p,p));
            cout<<ctrl.back().first<<endl;
            settings.popTag();
        }
        
        settings.popTag(); //pop position
    }
    else{
        ofLogError("Position file did not load!");
    }

    

    /*
     
     
     
     ＝＝＝＝＝＝＝＝＝＝＝ＦＡＣＥＴＲＡＣＫＩＮＧ＝＝＝＝＝＝＝＝＝＝＝
     
     
     
     */
    
    //ofSetDataPathRoot(ofFile(__BASE_FILE__));
    
    // Setup grabber
    grabber.setup(1280,720);
    
    // Setup tracker
    tracker.setup("shape_predictor_68_face_landmarks.dat");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    
    // Update tracker when there are new frames
    if(grabber.isFrameNew()){
        tracker.update(grabber);
        
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    /*
     
     
     
    ＝＝＝＝＝＝＝＝＝＝＝ＰＯＲＴＲＡＩＴ＝＝＝＝＝＝＝＝＝＝＝
     
     
     
     */
    
    
    
    tex.draw(0,0);
    
    
    
    
    /*
     
     
     
     ＝＝＝＝＝＝＝＝＝＝＝ＦＡＣＥＴＲＡＣＫＩＮＧ＝＝＝＝＝＝＝＝＝＝＝
     
     
     
     */
    
    
    
    
    
    
    
    // Draw camera image
    //grabber.draw(0, 0);
    //ofVec3f templ_point;
    //cameraFace.assign(30,templ_point);
    // Draw tracker landmarks
    //tracker.drawDebug();
    auto instances = tracker.getInstances();
    for(int i = 0; i < instances.size(); i++) {
        auto landmarks = instances[i].getLandmarks();
        auto points = landmarks.getImagePoints();
        cameraFace.clear();
        for (int j = 0; j < points.size(); j++) {
            if((j == 0)||(j == 4)||(j == 8)||(j == 12)||(j == 16)||(j == 27)||(j == 31)||(j == 33)||(j == 35)||(j == 36)||(j == 37)||(j == 39)||(j == 41)||(j == 42)||(j == 43)||(j == 45)||(j == 47)||(j == 48)||(j == 54)||(j == 57)||(j == 66)){
                cameraFace.push_back(points[j]);
            }
        }
        
    }
    
    // Draw estimated 3d pose
    //tracker.drawDebugPose();
    
    //check if face face to moniter
    for(auto face : tracker.getInstances()){
        ofQuaternion faceRotate = face.getPoseMatrix().getRotate();
        //cout<<faceRotate[0]<<endl;
        if((faceRotate[0]<0.003 && faceRotate[0]>-0.003)){
            //cout<<face.getPoseMatrix().getRotate()<<endl;
            if(cameraFace.size()>=21){
                
                
                ofPoint basePoint1 = cameraFace.at(0);
                ofPoint basePoint2 = cameraFace.at(4);
                ofPoint setPosition(985,244,0);
                ofPoint scaleTarget(1007,480,0);
                ofPoint setScale(setPosition.distance(scaleTarget));
                
                
                float angle = (ofVec3f(1,0,0).angle(basePoint2-basePoint1));
                
                
                if(basePoint2.y>basePoint1.y)
                    angle*=(-1);
                //cout<<angle<<endl;
                
                
                for(int j=0; j<21; j++){
                    ofPoint p = cameraFace.at(j) -basePoint1;
                    p.rotate(angle+90, ofVec3f(0,0,1));
                    p = ((p/basePoint2.distance(basePoint1))*setScale)+setPosition;
                    cameraFace.at(j) = p;
                        if(ctrl.size()<20)
                        ofDrawBitmapString(j,cameraFace.at(j));
                }
                
                
                if(ctrl.size()>=28){
                    //if(countFrame%3 == 0)
                    ctrl.at(countFrame).second = cameraFace.at(countFrame);
                    imgWarp.calcDelta(ctrl);
                    imgWarp.genNewImg(image.getPixels(),dst,1);
                    tex.loadData(dst);
                    cout<<countFrame<<endl;
                }

                countFrame++;
                if(countFrame==21){
                    countFrame = 0;
//                    for(int j=0; j<21; j++){
//                        ofDrawCircle(cameraFace.at(j),3);
//                    }
                    cout<<"clean"<<endl;
                }
            }
        }
    }


    
    
    
    if(ctrl.size()<20)
    for(int i=0;i<ctrl.size();i++){
        ofNoFill();
        ofDrawCircle(ctrl.at(i).second,3);
    }


}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
//    //cout << "test   " << x << " , " << y << endl;
//    if(ctrl.size()>0){
//        for(int i = 0; i<ctrl.size(); i++){
//            int dis = sqrt(pow(x - ctrl.at(i).second.x,2)+pow(y - ctrl.at(i).second.y,2));
//            if(dis < 10){
//                
//                ctrl.at(i).second = ofPoint(x,y);
//                imgWarp.calcDelta(ctrl);
//                imgWarp.genNewImg(image.getPixelsRef(),dst,1);
//                tex.loadData(dst);
//            }
//        }
//    }
    //cout << ctrl.at(0).second<< "    " << ctrl.at(0).first <<  endl;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    
    
    
//    if (pointNum == 0) {
//        settings.addTag("settings");
//        settings.pushTag("settings");
//        cout<<"step1"<<endl;
//    }
//    if(ctrl.size()< 28){
//        ctrl.push_back(make_pair( ofPoint(x,y),ofPoint(x,y)));
//        cout<<ctrl.back().first<<endl;
//        settings.addTag("position");
//        settings.pushTag("position", pointNum);
//        settings.addValue("x", x);
//        settings.addValue("y", y);
//        settings.popTag();
//        cout<<"get it!"<<endl;
//    }
//    arr[pointNum][0] = x;
//    arr[pointNum][1] = y;
//    pointNum++;
//    
//    if (pointNum == 29) {
//        settings.popTag();
//        settings.saveFile("settings.xml");
//        cout << "Points saved in file!" << endl;
//    }

    //cout << "test   " << ctrl.back().first << ctrl.back().second <<  endl;
    
}
