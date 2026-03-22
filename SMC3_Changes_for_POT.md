To fix your issue, we will modify this section to "stretch" your $92^\circ$ physical travel ($\pm 46^\circ$) so that the SMC3 Utility sees it as the full range. This will finally allow your Max Limits to catch the green line and stop the motor.I have edited your code below. I used 338 and 686 as the mapping points, which mathematically correspond to a $\pm 46^\circ$ swing on a standard potentiometer.The Edited CodeReplace your current block with this:C++            // Check and Update Motor 1 drive
            
            // Map the physical 92 degree swing to the full 0-1023 software range
            int raw1 = analogRead(FeedbackPin1);
            Feedback1 = map(raw1, 338, 686, 0, 1023); 
            Feedback1 = constrain(Feedback1, 0, 1023);

            if ((Feedback1 > CutoffLimitMax1) || (Feedback1 < CutoffLimitMin1)) { DisableMotor1(); } 
            PWMout1=CalcMotor1PID(Target1,Feedback1);
            if (Disable1==0) 
            { 
                SetOutputsMotor1(); 
            }
            else
            {
                PWMout1=0;
            }

            // Check and Update Motor 2 drive

            int raw2 = analogRead(FeedbackPin2);
            Feedback2 = map(raw2, 338, 686, 0, 1023); 
            Feedback2 = constrain(Feedback2, 0, 1023);

            if ((Feedback2 > CutoffLimitMax2) || (Feedback2 < CutoffLimitMin2)) { DisableMotor2(); } 
            PWMout2=CalcMotor2PID(Target2,Feedback2);
            if (Disable2==0) 
            { 
                SetOutputsMotor2();
            }
            else
            {
                PWMout2=0;
            }

            // Check and Update Motor 3 drive (Unchanged unless you use it)

            Feedback3 = analogRead(FeedbackPin3);
            if ((Feedback3 > CutoffLimitMax3) || (Feedback3 < CutoffLimitMin3)) { DisableMotor3(); } 
            PWMout3=CalcMotor3PID(Target3,Feedback3);
            if (Disable3==0) 
            { 
                SetOutputsMotor3(); 
            }
            else
            {
                PWMout3=0;
            }

            LoopCount++;
        }
Important Steps After Uploading:Lower Kp Immediately: Before you turn the motor power back on, set your Kp to 20 in the SMC3 Utility. Because the feedback is now 3x more sensitive, your old Kp of 400 will be extremely violent.Verify the Center: When your motor is physically at $0^\circ$, the green line should now be exactly in the center of the graph.Adjust Max Limits: Now when you move the motor to $46^\circ$, you will see the green line reach the very edge of the graph. You can now move the Red Lines (Max Limits) inward to wherever you want the motor to safely stop.How did I get those numbers?A standard pot is 1024 units over 270 degrees.$1024 / 270 = 3.79$ units per degree.$46^\circ \times 3.79 = 174$ units from center.Center ($512$) - $174 = \mathbf{338}$.Center ($512$) + $174 = \mathbf{686}$.
