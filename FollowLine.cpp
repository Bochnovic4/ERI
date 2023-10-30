#include "NXT++.h"
#include <iostream>
#include <conio.h>
using namespace std;
using namespace NXT;

Comm::NXTComm comm;

//PRZED UŻYCIEM SPRZETU DOKŁADNIE SPRAWDZAMY: 
//DO KTÓRYCH PORTÓW WEJŚCIOWYCH (IN) PODŁĄCZONE SĄ SENSORY,\
//DO KTÓRYCH PORTÓW WYJŚCIOWYCH (OUT) PODŁĄCZONE SĄ SERVO-MOTORY,
//STARAMY SIĘ PISAC KOD Z MOŻLIWOŚCIĄ ZATRZYMANIA ROBOTA W KAŻDEJ CHWILI,
//ROBOT POWIENIEN BYC PRZEMIESZCZONY W MIEJSCE TESTÓW ZA POŚREDNICTWEM ZDALNEGO STEROWANIA,
//CZUJNIKI SA INICJOWANE PRZED PETLA NIESKONCZONA,
//CZUJNIKI SA DEAKTYWOWANE ZA PETLA NIESKONCZONA, LUB PO KOMENDZIE k,

int main()
{
	srand(time(NULL));

	cout << "Szukanie sygnalu... to moze zajac chwile.\n";
	if(NXT::OpenBT(&comm)) //initialize the NXT and continue if it succeeds
	{
		cout<< "Sygnal znaleziony!";
		cout<<"\nPoziom baterii = "<<NXT::BatteryLevel(&comm);
		cout<<"\nDostepna pamiec flash = "<<NXT::GetAvailableFlash(&comm);
		
		int x=30;//szybkość jazdy w przód i tył
		int xtemp=30;//potrzebna do sprawdzania czy predkosc sie zmienila

		int y=20;//szybkość obrotu w lewo, prawo
		int ytemp=20;//potrzebna do sprawdzania czy predkosc sie zmienila

		char last_value;//zapamietana ostatnia instrukcja sterująca

		//Przed uzyciem sprawdzamy numer portu do którego jest podlączony sensor, 
		//NXT::Sensor::SetTouch(&comm, IN_1);//inicjacja sensora dotyku

		//aktywacja czujnika koloru
		NXT::Sensor::SetLight(&comm, IN_3, 1) ;
		int color;
	    int darkColor;
		int lightColor;
		//aktywacja sonaru,
		NXT::Sensor::SetSonar(&comm, IN_1);

		//początek programu
		NXT::StartProgram(&comm,"program1");

		//zmienna używana do sterowania
		char decyzja;

		cout<<"\n ___Podstawowe sterowanie do uzupelnienia___";
		cout<<"\n A - jazda w lewo"; 
		cout<<"\n D - jazda w prawo"; 
		cout<<"\n W - jazda na wprost"; 
		cout<<"\n S - jazda do tylu"; 
		cout<<"\n U - odczyt z sonaru"; 
		cout<<"\n C - odczyt nasycenia koloru";
		cout<<"\n H - Zatrzymanie silnikow";
		cout<<"\n L - Follow line";
		cout<<"\n P -- Get Light Color";
		cout<<"\n O -- Get Dark Color";
		cout<<"\nK - Koniec Programu";

		do
		{
			//Przed uzyciem sprawdzamy numer portu do którego jest podlączony sensor, 
		/*
		if(NXT::Sensor::GetValue(&comm, IN_1)==1)
			{
			for(int i=1;i<1000;i=i+10)
				{
				NXT::PlayTone(&comm, i, 1);
				Wait(1);
				}
			}
		*/

			if(kbhit()==true)
			{
            
			decyzja=getch();
	
			if(decyzja=='N' || decyzja=='n')
			{
			x-=10; 
			}

			if(decyzja=='M' || decyzja=='m')
			{
			x+=10; 
			}

			if(decyzja=='V' || decyzja=='v')
			{
			y--; 
			}

			if(decyzja=='B' || decyzja=='b')
			{
			y++; 
			}
			//Przed uzyciem serwo sprawdzamy numer portu do którego jest podlączony,
			if(decyzja=='A' || decyzja=='a' || (ytemp!=y && last_value=='a'))
			{
			ytemp=y;
			last_value='a';
			NXT::Motor::SetForward(&comm, OUT_B, y);
			NXT::Motor::SetReverse(&comm, OUT_C, y);
			//cout<<"SonarValue = "<<NXT::Sensor::GetSonarValue(&comm, IN_4);
			}

			//Przed uzyciem serwo sprawdzamy numer portu do którego jest podlączony,
			if(decyzja=='D' || decyzja=='d' || (ytemp!=y && last_value=='d'))
			{
			ytemp=y;
			last_value='d';
			NXT::Motor::SetReverse(&comm, OUT_B, y);
			NXT::Motor::SetForward(&comm, OUT_C, y);
			}

			//Przed uzyciem serwo sprawdzamy numer portu do którego jest podlączony,
			if(decyzja=='W' || decyzja=='w' || (xtemp!=x && last_value=='w'))
			{
			xtemp=x;
			last_value='w';
			NXT::Motor::SetForward(&comm, OUT_B, x);
			NXT::Motor::SetForward(&comm, OUT_C, x);
			}

			//Przed uzyciem serwo sprawdzamy numer portu do którego jest podlączony,
			if(decyzja=='S' || decyzja=='s'  || (xtemp!=x && last_value=='s'))
			{
			xtemp=x;
			last_value='s';
			NXT::Motor::SetReverse(&comm, OUT_B, x);
			NXT::Motor::SetReverse(&comm, OUT_C, x);
			}

			//Przed uzyciem serwo sprawdzamy numer portu do którego jest podlączony,
			if(decyzja=='H' || decyzja=='h')
			{
			NXT::Motor::SetForward(&comm, OUT_B, 0);
			NXT::Motor::SetForward(&comm, OUT_C, 0);
			NXT::Motor::Stop(&comm, OUT_B, 0);
			NXT::Motor::Stop(&comm, OUT_C, 0);
			}
       
			//Przed uzyciem serwo sprawdzamy numer portu do którego jest podlączony,
			if(decyzja=='K' || decyzja=='k')
			{
			NXT::Motor::Stop(&comm, OUT_B, 0);
			NXT::Motor::Stop(&comm, OUT_C, 0);
			NXT::Motor::Stop(&comm, OUT_A, 0);
			NXT::Sensor::SetSonarOff(&comm, IN_1);			
			NXT::Sensor::SetColorOff(&comm,IN_2);

			break;
			}
			
			//Przed uzyciem sprawdzamy numer portu do którego jest podlączony sensor, 
			if(decyzja=='U' || decyzja=='u')
			{
			cout<<" Wskazanie sonaru = "<<NXT::Sensor::GetSonarValue(&comm, IN_1);
			}

			//Przed uzyciem sprawdzamy numer portu do którego jest podlączony sensor, 
			if(decyzja=='C' || decyzja=='c')
			{
				color=NXT::Sensor::GetValue(&comm,IN_3);
				Wait(50);
				cout<<"\nNasycenie swaitla = "<<color;
			}
			if(decyzja=='P' || decyzja=='p')
			{
				darkColor=NXT::Sensor::GetValue(&comm,IN_3);
				Wait(50);
				cout<<"\nWartosc czarnego = "<<darkColor;
			}
			if(decyzja=='O' || decyzja=='o')
			{
				lightColor=NXT::Sensor::GetValue(&comm,IN_3);
				Wait(50);
				cout<<"\nWartosc jasnego = "<<lightColor;
			}
			if (decyzja == 'L' || decyzja == 'l') {
				int threshold = (darkColor + lightColor) / 2;
				int color;

				while (true) {
					color = NXT::Sensor::GetValue(&comm, IN_3);
					NXT::Motor::SetForward(&comm, OUT_B, x);
					NXT::Motor::SetForward(&comm, OUT_C, x);
					if (color < threshold) {
						// Move left when the sensor sees a dark line
						NXT::Motor::SetForward(&comm, OUT_B, y);
						NXT::Motor::SetReverse(&comm, OUT_C, y);
					} else {
						// Move right when the sensor sees a light area
						NXT::Motor::SetReverse(&comm, OUT_B, y);
						NXT::Motor::SetForward(&comm, OUT_C, y);
					}
				}
			}
			continue;
			}
		}while(decyzja!='k' && decyzja!='K');

		NXT::StopProgram(&comm);
		
	}
	NXT::Close(&comm); //close communication with NXT

	cout<<"\n\nPress ENTER to close program";
	getchar();

	return 0;
}


//opcje dotyczace Mindstorm NXT v. 1.0

//if(decyzja=='L' || decyzja=='l')
// {
//  NXT::Sensor::SetLight(&comm, IN_3, 1);
//  Wait(50);
//  cout<<"LightValue = "<<NXT::Sensor::GetValue(&comm, IN_3);
//  Wait(50);
//  NXT::Sensor::SetLight(&comm, IN_3, 0);
// }

	//if(decyzja=='O' || decyzja=='o')
// {
// NXT::Sensor::SetSound(&comm, IN_2);
// Wait(50);
// cout<<"SoundValue = "<<NXT::Sensor::GetValue(&comm, IN_2);
// }

