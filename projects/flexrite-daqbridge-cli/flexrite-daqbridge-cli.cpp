// flexrite-daqbridge-cli.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <string>
#include <stdio.h>
#include "ros.h"
#include <geometry_msgs/Twist.h>
#include <windows.h>
#include <conio.h>
#include "quickDAQ.h"
#include "matrixops.h"
#include "utilities.h"

#define timeStep 10
#define samplingRate 100
#define NIDEVSLOT 6

#define useHWtiming 0

using std::string;
using namespace std;
using namespace matrixops;

int main()
{
    ros::NodeHandle nh;
    FILE* ros_server_ipConf_filePtr = NULL;
    char ros_server_ip[20];

    cout << "-----------------------------------\n";
    cout << "FlexRite DAQ-bridge over ROS serial\n";
    cout << "-----------------------------------\n";
    cout << "NI USB-6343 Pin Reference:\n";
    cout << "\tJR3 Fx  [Brown   ] -> AI0    [Pin 1]\n";
    cout << "\tJR3 Fy  [Red     ] -> AI1    [Pin 4]\n";
    cout << "\tJR3 Fz  [Orange  ] -> AI2    [Pin 7]\n";
    cout << "\tJR3 Mx  [Yellow  ] -> AI8    [Pin 2]\n";
    cout << "\tJR3 My  [Green   ] -> AI9    [Pin 5]\n";
    cout << "\tJR3 Mz  [Blue    ] -> AI10   [Pin 8]\n";
    cout << "\tJR3 GND [Black x6] -> AI GND [Pin 3]\n\n";
    //cout << "\tMoCap Trigger [BNC signal] <- AO0    [Pin 15]\n";
    //cout << "\tMoCap ground  [BNC ground] <- AO GND [Pin 16]\n";
    cout << "-----------------------------------\n";

    fopen_s(&ros_server_ipConf_filePtr, "rosserial_server_ip.conf", "r");
    fscanf_s(ros_server_ipConf_filePtr, "%s\n", ros_server_ip, 20);
    printf("\nConnecting to ROS master at %s\n", ros_server_ip);
    nh.initNode(ros_server_ip);

    printf("Advertising 'JR3_wrench' message\n");
    geometry_msgs::Twist twist_msg;
    ros::Publisher cmd_vel_pub("JR3_wrench", &twist_msg);
    nh.advertise(cmd_vel_pub);

    printf("Starting Data acquisition! Press ESC or CTRL+C to stop data transmission...\n\n");
    double wrench[6]        = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double offsetVoltage[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double rawVoltage[6]    = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    quickDAQinit();
    pinMode(NIDEVSLOT, ANALOG_IN, 0);
    pinMode(NIDEVSLOT, ANALOG_IN, 1);
    pinMode(NIDEVSLOT, ANALOG_IN, 2);
    pinMode(NIDEVSLOT, ANALOG_IN, 8);
    pinMode(NIDEVSLOT, ANALOG_IN, 9);
    pinMode(NIDEVSLOT, ANALOG_IN, 10);
    //pinMode(NIDEVSLOT, ANALOG_OUT, 0);

    setSampleClockTiming((samplingModes) ((useHWtiming==0) ? ON_DEMAND : HW_CLOCKED),
        samplingRate,
        DAQmxClockSource,
        (triggerModes)DAQmxTriggerEdge,
        DAQmxNumDataPointsPerSample,
        TRUE);

    bool liveFlag = TRUE;
    char keyStroke;

    // Start DAQ and calibrate JR3
    quickDAQstart();
    syncSampling();
    readAnalog_extBuf(6, offsetVoltage);
    if (useHWtiming) syncSampling();

    while (liveFlag != FALSE)
    {
        // Read raw voltages and remove offset. Then compute wrench...
        readAnalog_extBuf(NIDEVSLOT, rawVoltage); // read from DAQ
        vectorSub<double, 6>(rawVoltage, rawVoltage, offsetVoltage); // remove offset
        matrixVectorMultiply<double, 6>(wrench, JR3_calibrationMatrix, rawVoltage); // compute wrench from voltages

        twist_msg.linear.x  = wrench[0];
        twist_msg.linear.y  = wrench[1];
        twist_msg.linear.z  = wrench[2];
        twist_msg.angular.x = wrench[3];
        twist_msg.angular.y = wrench[4];
        twist_msg.angular.z = wrench[5];
        cmd_vel_pub.publish(&twist_msg);
        nh.spinOnce();

        printf("Fx: %+4.2fN, Fy: %+4.2fN, Fz: %+4.2fN | Mx: %+4.2fNm, My: %+4.2fNm, Mz: %+4.2fNm\r",
                wrench[0],wrench[1],wrench[2],wrench[3],wrench[4],wrench[5]);

        if (_kbhit()) {
            keyStroke = _getch();
            if (int(keyStroke) == 27) {
                liveFlag = FALSE;
            }
        } else {
            if (useHWtiming)
                syncSampling();
            else 
                Sleep(timeStep);
        }
    }

    if (useHWtiming) syncSampling();
    quickDAQstop();
    quickDAQTerminate();
    printf("\nAll done! Closing program...\n");


    return 0;
}

