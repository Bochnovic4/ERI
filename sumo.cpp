#include "NXT++.h"
#include <iostream>
#include <conio.h>
using namespace std;
using namespace NXT;

Comm::NXTComm comm;

int main()
{
    srand(time(NULL));

    cout << "Searching for signal... This may take a moment.\n";
    if (NXT::OpenBT(&comm)) // Initialize the NXT and continue if it succeeds
    {
        cout << "Signal found!";
        cout << "\nBattery Level = " << NXT::BatteryLevel(&comm);
        cout << "\nAvailable Flash Memory = " << NXT::GetAvailableFlash(&comm);

        int x = 30; // Speed for forward and backward motion
        int xtemp = 30; // Needed for checking speed changes

        int y = 20; // Speed for turning left and right
        int ytemp = 20; // Needed for checking speed changes

        char last_value; // Remember the last control command

        // Activate the color sensor
        NXT::Sensor::SetLight(&comm, IN_3, 1);
        int color;
        int darkColor;
        int lightColor;

        // Activate the ultrasonic sensor
        NXT::Sensor::SetSonar(&comm, IN_1);

        // Start the program
        NXT::StartProgram(&comm, "program1");

        // Control variable
        char decyzja;

        cout << "\n ___Basic Controls___";
        cout << "\n A - Turn Left";
        cout << "\n D - Turn Right";
        cout << "\n W - Move Forward";
        cout << "\n S - Move Backward";
        cout << "\n U - Read Ultrasonic Sensor";
        cout << "\n C - Read Light Sensor";
        cout << "\n H - Stop Motors";
        cout << "\n K - End Program";

        do
        {
            if (kbhit())
            {
                decyzja = getch();

                if (decyzja == 'N' || decyzja == 'n')
                {
                    x -= 10;
                }

                if (decyzja == 'M' || decyzja == 'm')
                {
                    x += 10;
                }

                if (decyzja == 'V' || decyzja == 'v')
                {
                    y--;
                }

                if (decyzja == 'B' || decyzja == 'b')
                {
                    y++;
                }

                if (decyzja == 'A' || decyzja == 'a' || (ytemp != y && last_value == 'a'))
                {
                    ytemp = y;
                    last_value = 'a';
                    NXT::Motor::SetForward(&comm, OUT_B, y);
                    NXT::Motor::SetReverse(&comm, OUT_C, y);
                }

                if (decyzja == 'D' || decyzja == 'd' || (ytemp != y && last_value == 'd'))
                {
                    ytemp = y;
                    last_value = 'd';
                    NXT::Motor::SetReverse(&comm, OUT_B, y);
                    NXT::Motor::SetForward(&comm, OUT_C, y);
                }

                if (decyzja == 'W' || decyzja == 'w' || (xtemp != x && last_value == 'w'))
                {
                    xtemp = x;
                    last_value = 'w';
                    NXT::Motor::SetForward(&comm, OUT_B, x);
                    NXT::Motor::SetForward(&comm, OUT_C, x);
                }

                if (decyzja == 'S' || decyzja == 's' || (xtemp != x && last_value == 's'))
                {
                    xtemp = x;
                    last_value = 's';
                    NXT::Motor::SetReverse(&comm, OUT_B, x);
                    NXT::Motor::SetReverse(&comm, OUT_C, x);
                }

                if (decyzja == 'H' || decyzja == 'h')
                {
                    NXT::Motor::SetForward(&comm, OUT_B, 0);
                    NXT::Motor::SetForward(&comm, OUT_C, 0);
                    NXT::Motor::Stop(&comm, OUT_B, 0);
                    NXT::Motor::Stop(&comm, OUT_C, 0);
                }

                if (decyzja == 'K' || decyzja == 'k')
                {
                    NXT::Motor::Stop(&comm, OUT_B, 0);
                    NXT::Motor::Stop(&comm, OUT_C, 0);
                    NXT::Motor::Stop(&comm, OUT_A, 0);
                    NXT::Sensor::SetSonarOff(&comm, IN_2);
                    NXT::Sensor::SetColorOff(&comm, IN_3);

                    break;
                }

                if (decyzja == 'U' || decyzja == 'u')
                {
                    cout << "Ultrasonic Value = " << NXT::Sensor::GetSonarValue(&comm, IN_1) << " cm";
                }

                if (decyzja == 'C' || decyzja == 'c')
                {
                    color = NXT::Sensor::GetValue(&comm, IN_3);
                    Wait(50);
                    cout << "\nLight Intensity = \n" << color;
                }

                if (decyzja == 'P' || decyzja == 'p')
                {
                    darkColor = NXT::Sensor::GetValue(&comm, IN_3);
                    Wait(50);
                    cout << "\nDark Color Value = " << darkColor;
                }

                if (decyzja == 'O' || decyzja == 'o')
                {
                    lightColor = NXT::Sensor::GetValue(&comm, IN_3);
                    Wait(50);
                    cout << "\nLight Color Value = " << lightColor;
                }
            }
        } while (decyzja != 'k' && decyzja != 'K');

        NXT::StopProgram(&comm);
    }

    NXT::Close(&comm);

    cout << "\n\nPress ENTER to close the program";
    getchar();

    return 0;
}
