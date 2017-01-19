//FACE-EYES DETECTION BUILD v1.0 - Final Version - Written by Freddy Ahumah & Mirko Rossi from ITT Galileo Galilei

#include "C:\opencv\build\include\opencv2\highgui\highgui.hpp"	//LIBRERIE DI OPENCV UTILIZZATE PER L'OBJECT DETECTING E LA GESTIONE DELLE TELECAMERE
#include "C:\opencv\build\include\opencv2\objdetect.hpp"		//----------------------------------------------
#include "C:\opencv\build\include\opencv2\imgproc.hpp"			//----------------------------------------------
#include <iostream>												//LIBRERIE STANDARD NECESSSARIE A C++ PER FUNZIONARE
#include <stdio.h>												//----------------------------------------------
#include <ctime>												//----------------------------------------------
#include <stdlib.h>												//----------------------------------------------
#include <Windows.h>											//----------------------------------------------
#include <MMSystem.h>											//LIBRERIE PER IL CARICAMENTO DEI FILE AUDIO WAV
#include <mciapi.h>												//----------------------------------------------
#pragma comment(lib, "Winmm.lib")								//----------------------------------------------

using namespace std;
using namespace cv;

//CASCADE CLASSIFIER
CascadeClassifier face_cascade;									//CREA UN CLASSIFIER PER IL CONTROLLO CASCADE DELLA FACCIA E DEGLI OCCHI
CascadeClassifier eyes_cascade;									//----------------------------------------------
//VARIABILI GLOBALI
String ip[2];													//CONTIENE LE STRINGHE DEGLI INDIRIZZI IP DELLE TELECAMERE, INSERITI DALL'UTENTE
bool  FirstSound = true;
time_t t1, t2,ts1,ts2;											//T1 e T2 SERVONO PER PARAGONARE IL TEMPO DI DELAY MENTRE TS1 e TS2 SERVONO PER CREARE UN RITARDO TRA I SUONI
int audio;														//SCELTA DELL'AUDIO 1)BEEP 2)FILE WAV IN DIRECTORY PROGETTO
//PROTOTIPI FUNZIONI
void detectFaces(Mat frame, float delay, int min);				//TROVA LA FACCIA + OCCHI ---> Mat frame = array che contiene i frame video, Delay = ritardo deciso dall'utente
int LoadControlCascade();										//CONTROLLO DI CARICAMENTO DELLE CASCADE NECCESSARIE A INDIVIDUARE OCCHI E FACCIA
string Cams(int contatore);										//UNISCE LE STRINGHE NECESSARIE A CONNETTERSI A TELECAMERE AXIS DELL'AZIENDA ITEL
int Menu(int);													//STAMPA UN MENU PRINCIPALE DALLA QUALE SCEGLIERE LE OPZIONI
float Delay(int camera);										//PERMETTE IL RITARDO NEL MOMENTO IN CUI LA TELECAMERAIP/WEBCAM NON VEDE PIU' FACCE
int main(int argc, char* argv[])
{
	int choose=0, choose_ip=0;										//VARIABILI DI SCELTA PER TIPO DI TELECAMERA (choose) E PER LE TELECAMERE IP (choose_ip)
	string StreamAdd[2]; 										//ARRAY DI RIFERIMENTO DOVE VIENE RECEPITA LA SOMMA DELLE STRINGHE NECESSARIE PER CONNETTERSI ALLE TELECAMERE IP
	int contatore = 0;											//CONTA QUANTI INDIRIZZI CI SONO NELL'ARRAY CONTENENTE GLI INDIRIZZI IP (IP[])
	float delay=0;												//DETERMINA I SECONDI DI RITARDO DECISI DALL'UTENTE DA QUANDO NON TROVA PIU' UNA FACCIA

	while (1)
	{
		system("cls");
		cout << "FACE-EYE DETECTION SOFTWARE v1.0 written by Freddy Ahumah and Rossi Mirko" << endl;
		cout << "Using OpenCV " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << "." << CV_SUBMINOR_VERSION << endl;
		choose = 0;
		choose = Menu(choose);

		FirstSound = true;
		//----------------------------------------------------------------------
		//							TELECAMERA IP
		//----------------------------------------------------------------------
		if (choose == 1)
		{
			if (contatore == 0)
			{		
				StreamAdd[contatore] = Cams(contatore);											//ACQUISISCE LA STRINGA PER LA CONNESSIONE ALLA TELECAMERA IP
				contatore++;																	//INDICE NMR INDIRIZZI
			}
			else if (contatore == 1)															
			{
				cout << endl << "Do you want to insert a new address in the memory? (y/n) ";	//ACQUISIZIONE NUOVO INDIRIZZO IP (MAX. 2) PER LA IPCAM
				char ans;
				cin >> ans;
				switch (ans)
				{
				case 'y':
				{
					StreamAdd[contatore] = Cams(contatore);
					contatore++;
					break;
				}
				case 'n':
					break;
				default:
					cout << endl << "Wrong answer. Please try again";
				};
			}
			while (1)
			{
				cout << endl << "Select the type of audio:\n1: Beep\n2: 'Guardare lo schermo'\nScelta: "; //SELEZIONE TIPO ALLARME SONORO
				cin >> audio;
				if (audio < 1 || audio>2)
					cout << endl << "Error! Please try again." << endl;
				else
					break;
			}
			cout << endl << endl << "Available IP Cams:";
			for (int c = 0; c < contatore; c++)										//MOSTRA LE IPCAMS FUNZIONANTI ALLA QUALE POSSIAMO ACCEDERE
			{
				cout << endl << c + 1 << ": " << ip[c];
			}
			while (1)
			{
				cout << endl << "Choice (1 or 2) : ";								//PERMETTE LA SCELTA DELLA IPCAM ALLA QUALE ACCEDERE
				cin >> choose_ip;
				if (choose_ip>contatore || choose_ip<0 )
					cout << endl << "Error! Please try again." << endl;
				else
					break;
			}
			delay = Delay(choose);
			Mat frame_ip;
			VideoCapture cap(StreamAdd[choose_ip - 1]);								//PASSA ALLA CLASSE VIDEOCAPTURE LA STRINGA CHE PERMETTE LA CONNESSIONE
			cap.open(StreamAdd[choose_ip - 1]);										//APRE LA CAMERA IN BACKGROUND SULLA STRINGA FORNITA
			namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);								//CREA UNA FINESTRA CHIAMATA "MyVideo"SULLA QUALE POTREMO VISIONARE I FRAMES DELLE CAMS
			if (!cap.isOpened())													//CONTROLLO SULL' INSUCCESSO IN APERTURA DELLA TELECAMERA
			{
				CV_Assert("Cam open failed");
				cout << endl << "Cam open failed, the program will restart";
				cvDestroyWindow("MyVideo");											//SE LA CAMERA FALLISCE AD APRIRSI, CHIUDE LA FINESTRA "MyVideo"
				ip[contatore] = "";
				contatore--;
			}
			LoadControlCascade();													//CARICA E CONTROLLA IL SUCCESSO DEL CARICAMENTO DELLE CASCADE
			t1 = time(NULL);														//PRENDE IL TEMPO DI PARTENZA PRIMA DI AVVIARE LA FUNZIONE PRINCIPALE PER CRONOMETRARE IL TEMPO
			while (cap.read(frame_ip))													//CONTROLLO SUL SUCESSO DI LETTURA DEL FRAME
			{
				detectFaces(frame_ip, delay, 1);										//DAL MOMENTO IN CUI LEGGO UN FRAME AVVIO LA FUNZIONE PRINCIPALE
				if (waitKey(35) == 27)												//PREMO ESC PER USCIRE DALLA FINESTRA DI VISUALIZZAZIONE DELLA CAM
				{
					cout << "Esc key is pressed by user" << endl;
					cvDestroyWindow("MyVideo");
					break;
				}
			}
		}
		//----------------------------------------------------------------------
		//								WEBCAM
		//----------------------------------------------------------------------
		if (choose == 2)
		{
			delay = Delay(choose); 
			Mat frame;
			int WebcamNum[5] = { 0,1,2,3,4 };
			int TestCam = 0;
			VideoCapture kap(WebcamNum[TestCam]); // open the video camera no. 0
			while (1)
			{
				cout << endl << "Select the type of audio:\n1: Beep\n2: 'Guardare lo schermo'\nScelta: "; //SELEZIONE TIPO ALLARME SONORO
				cin >> audio;
				if (audio < 1 || audio>2)
					cout << endl << "Error! Please try again." << endl;
				else
					break;
			}
			cout << "Available cams: " << endl;
			while (1)
			{
				kap.open(WebcamNum[TestCam]);
				if (kap.read(frame))
				{
					cout << "Cam num : "<<WebcamNum[TestCam] << endl;
					TestCam++;
				}
				else
					break;
			}
			TestCam--; //RIPORTO IL CONTATORE DELLE WEBCAM FUNZIONANTI AL VALORE REALE 
			while (1)
			{
				cout << endl <<endl << "Which webcam do you want to display? (from 0 to" << TestCam <<")"<< endl;
				int FunctioningCam = 0;
				cout << "Choice: ";
				cin >> FunctioningCam;
				if (FunctioningCam > TestCam)
					cout <<endl<< "Error! There is no camera for the value " << FunctioningCam << ", please try again! ";
				else
				{
					kap.open(FunctioningCam);
					break;
				}
			}
			if (!kap.isOpened())
				CV_Assert("Cam open failed");
			kap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
			kap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
			namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
			LoadControlCascade();
			t1 = time(NULL);
			while (kap.read(frame))
			{
				detectFaces(frame, delay, 1);
				if (waitKey(35) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				{
					cout << "Esc key is pressed by user" << endl;
					cvDestroyWindow("MyVideo");
					break;
				}
			}
		}
		if (choose == 3)
			break;
		if (choose > 3 || choose < 1)
		{
			cout << "Error: You have insert a wrong value! Try again!" << endl;
			Sleep(2000);
		}
	}
	return 0;
}
//----------------------------------------------------------------------
//						FUNZIONI UTILIZZATE
//----------------------------------------------------------------------
void detectFaces(Mat frame, float delay, int min)
{
	vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	size_t i;
	bool face = false;
	//cout << faces.size()<<endl;
	for (i = 0; i < faces.size(); i++)
	{
		if (audio == 1)//SCELTA AUDIO
			if (t2 - t1 > delay)
			{
				ts2 = time(NULL);
				if (FirstSound)
				{
					ts1 = time(NULL);//prendo il tempo per far aspettare tra un suono e l'altro
					Beep(523, 500);
					FirstSound = false;
				}
				if (ts2 - ts1>10)
				{
					ts1 = time(NULL);
					Beep(523, 500);
				}
			}
			if (audio == 2)
				if (t2 - t1 > delay)//Prendo tempo sistema per calcolare in base al ritardo gestito dall'utente
				{
					ts2 = time(NULL);
					if (FirstSound)
					{
						ts1 = time(NULL);
						
						PlaySound(L"voicewav.wav", NULL, SND_ASYNC);
						FirstSound = false;
					}
					if (ts2 - ts1>10)
					{
						ts1 = time(NULL);
						
						PlaySound(L"voicewav.wav", NULL, SND_ASYNC);
					}
				}
		//PARTE: FACCIA
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
		face = true;
		//NUOVA PARTE: OCCHI
		Mat faceROI = frame_gray(faces[i]);
		vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
		t1 = time(NULL);
	}
	t2 = time(NULL);
//	cout << t2 <<" - " <<t1 << " = " << t2-t1 << " > " << delay << endl << endl;
	imshow("MyVideo", frame);
	if(audio==1)
		if (t2 - t1 > delay)
		{
		
			ts2 = time(NULL);
			if (FirstSound)
			{
				ts1 = time(NULL);
				Beep(523, 500);
				FirstSound = false;
			}
			if(ts2-ts1>10)
			{
				ts1 = time(NULL);
				Beep(523, 500);
			}
		}
		if(audio==2)
			if (t2 - t1 > delay)
			{
				ts2 = time(NULL);
				if (FirstSound)
				{
					ts1 = time(NULL);
					PlaySound(L"voicewav.wav", NULL, SND_ASYNC);
					waitKey(3000);
					FirstSound = false;
				}
				if (ts2 - ts1>10)
				{
					ts1 = time(NULL);
					PlaySound(L"voicewav.wav", NULL, SND_ASYNC);

				}
			}
}

int LoadControlCascade()
{
	face_cascade.load("haarcascade_frontalface_alt.xml");
	eyes_cascade.load("haarcascade_eye_tree_eyeglasses.xml");
	if (!face_cascade.load("haarcascade_frontalface_alt.xml"))
	{
		cout << "--(!)Error loading face cascade\n";
		return -1;
	}
	if (!eyes_cascade.load("haarcascade_eye_tree_eyeglasses.xml"))
	{
		cout << "--(!)Error loading eyes cascade\n";
		return -1;
	}
	return NULL;
}
string Cams(int contatore)
{
	string http = "http://";
	string fine = "/video.mjpg";
	string ipaddress;
	//string userpwd = "http://root:Itel89bz@";
	//string modres = "/axis-cgi/mjpg/video.cgi?resolution=320x240&req_fps=5&.mjpg";



	cout << endl << "Insert the ip address of the camera that you want to look at [x.x.x.x]: ";
	cin >> ipaddress;
	ip[contatore] = ipaddress;


	return (http + ipaddress + fine);
	//return (userpwd + ipaddress + modres);
	//"http://root:Itel89bz@192.168.100.10/axis-cgi/mjpg/video.cgi?resolution=640x480&req_fps=30&.mjpg",
}
int Menu(int choose)
{
	cout << endl << "Main menu" << endl;
	cout << "1) Select IP camera and watch it " << endl;
	cout << "2) Select webcam and watch it " << endl;
	cout << "3) Exit the software" << endl << endl;
	cout << "Choice: ";
	cin >> choose;
	return choose;
}
float Delay(int camera)
{
	cout << endl << "Insert time of delay (in seconds)  of the alarm after no tracking: ";
	float x;
	cin >> x;
	return x;
}
