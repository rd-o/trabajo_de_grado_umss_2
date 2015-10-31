#include "BaseCon.hpp"

BaseCon::BaseCon(bool w, TrackbarCallback tc, bool robotEnable){
    if(!robotEnable){ 
        camara = new Camara(false);
    }else{
        camara = new Camara(true);
    }

    robot = new Robot(!robotEnable);
    initCW(robot, ENC1_1, ENC1_2, ENC2_1, ENC2_2);

    window = w;
    if(window)
        createWindow(tc);
}

/**
  * Recibe blobs y los procesa en el motor de inferencia
  */
void BaseCon::kb_inference (){
    bool avanzarR = true;
    bool girarD = false; 
    bool girarI = false; 
    bool cont;
    bool inclinado = false;
    double distancia;
    double distancia_v;
    float *angle;

    Mat frame;
    if(camara->cap.isOpened() || !frame.empty()){
        camara->cap >> frame;
    }else{
        cout << "No se puede obtener frame" << endl;
        return;
    }

    CBlobResult blobs = camara->findBlobs(frame);


	for(int i=0;i<blobs.GetNumBlobs();i++){
        cont = false;
		CBlob blob = blobs.GetBlob(i);
        Rect bbox = blob.GetBoundingBox();
        //si esta dentro de un rango de area (no muy grande o muy pequeno), 
        //entonces continua
        if( blob.Area(PIXELWISE) < camara->min_blob_area || 
            blob.Area(PIXELWISE) > camara->max_blob_area){
            continue;
        }
       
        //para hallar si el objeto esta al fondo 
        int fondo = bbox.y + bbox.height;

        if(window){
            //if (!cont) 
            camara->addBlobToImg(frame, blob);
	        imshow("blobs", frame);
        }


        distancia = utils::calcular_distancia(camara->grados, 
                    SCREEN_SIZE_Y - bbox.br().y);
        cout << "distancia " << distancia << " ";

        if(bbox.x < (camara->width / 2)){
            //blob al lado izquierdo de la pantalla
            //distancia medida desde el borde izquierdo de la pantalla hasta el 
            //borde derecho del blob
            distancia_v = utils::calcular_distancia_horizontal(distancia, 
                          bbox.x + bbox.width);
            cout << "Caso 1: " << bbox.x + bbox.width << " ";
            if(distancia < 20){
                girarD = true;
            }
        }else{
            //blob al lado derecho de la pantalla
            //distancia medida desde el borde izquierdo de la pantalla hasta el 
            //borde izquierdo del blob
            distancia_v = utils::calcular_distancia_horizontal(distancia, bbox.x);
            cout << "Caso 2: " << bbox.x << " ";
            if(distancia < 20){
                girarI = true;
            }
        }
        cout << "distancia_v: " <<distancia_v << " ";

        if(window){
            camara->addDistanciaBlob(distancia, blob, frame);
            camara->addAreaBlob(blob.Area(PIXELWISE), blob, frame);
        }
    }

    if(robot->getAngles(angle)){
        float inclinacion = angle[1] * 180/M_PI;
        if(inclinacion < -20){
            inclinado = true;
        }
    }

    if(inclinado){
        robot->retroceder(1000);
        robot->girarI(500);
        robot->avanzar();
    }else{
        if(girarI && !girarD){
            robot->girar(20);
        }else if (girarD && !girarI){
            robot->girar(-20);    
        }else if(!girarI && !girarD){
            robot->avanzar();
        }else if(girarD && girarI){
            //numero aleatorio
            std::srand(std::time(0));         
            int random_variable = std::rand();

            //preguntar si es par o impar y girar a un sentido segun el 
            //resultado
            if((random_variable % 2) == 0){
                robot->girar(50);
            }else{
                robot->girar(-50);
            }
        }
    }
}
void BaseCon::on_slider(int, void *) { }
void BaseCon::on_grados(int, void *) { 
    camara->mover(camara->grados);
}

/**
  * Ventana para hacer pruebas 
  */
void BaseCon::createWindow(TrackbarCallback tc){
    namedWindow("blobs");

    createTrackbar("min blobs area", "blobs", &camara->min_blob_area, 2000, tc);
    createTrackbar("max blobs area", "blobs", &camara->max_blob_area, 2000, tc);
    createTrackbar("grados camara", "blobs", &camara->grados, 180, tc);
}
