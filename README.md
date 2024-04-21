# flexrite-daqbridge
This PoC bridges NI-DAQmx data collection to ROS over rosserial.

## Prerequisites
- NI-DAQmx software drivers ([linked here](https://www.ni.com/en/support/downloads/drivers/download.ni-daq-mx.html), already installed)
- Visual Studio 2022 IDE community edition ([linked here](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false))

## Instructions for use
- Install the prerequisite software.
- Download the code from the GitHub repo.
- Connect the Ethernet cables between the Windows and Linux computers.
- Identify the IP address of the Ethernet connection on the Windows computer.
- Set the IP address of the Ethernet connection on the Linux computer to be one more than the value on the Windows computer.
- Optionally, you can test the connection from the Windows computer to the Linux computer using the command `ping <Linux Ethernet IP>` replacing the `<Linux Ethernet IP>` with the Ethernet IP of the Linux computer that you just set.
- Apply the Linux Ethernet IP address to the `ros_server_ip.conf` text file in the `projects\flexrite-daqbridge-cli` directory of this repo on the Windows computer.
- From the same `projects\flexrite-daqbridge-cli` directory within the code, open the `flexrite-daqbridge-cli.sln` file in Visual Studio 2022 IDE which was installed as a prerequisite.
- From Visual Studio 2022 IDE, open the Solution Explorer by hitting `CRTL+ALT+L` and open the `flexrite-daqbridge-cli.cpp` file under `Source Files`.
- Should you want to change the sampling rate from the default 100 Hz to 1000 Hz, change the value of `samplingRate` to `1000` in line 17. Also in line 16, change `timeStep` to `1`.
- Before you run the code on the Windows computer, ensure that rosserial server is running on the Linux box.
- To compile and run the program, hit `CTRL+F5`.
- The program will start, connect to the rosserial server on the Linux box.
- The program will automatically calibrate the JR3 and start sending data. So, start this program after you position the JR3 sensor next to the cadaver hand.
- You can hit `ESC` or `CTRL-C` to quit the program at any time. You can also hit the Close button on the top right corner of the window with your mouse to close the program.
- Repeat for every protocol and every hand.

## LICENSE
Distributed under the Mozilla Public License.
