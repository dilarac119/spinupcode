
#include "main.h"

//#include <include/okapi/api/chassis/controller/domChassisController.hpp>



//auto enc = ADIEncoder({1, 'A', 'B'}, false);


//1. Store the current encoder values in local variables



/*
//double ogVal = controllerGet();
ChassisControllerBuilder()
    .withMotors(1, -2) // left motor is 1, right motor is 2 (reversed)
    // green gearset, 4 inch wheel diameter, 11.5 inch wheel track
    .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
    .withSensors(
        ADIEncoder{'A', 'B'}, // left encoder in ADI ports A & B
        ADIEncoder{'C', 'D', true},  // right encoder in ADI ports C & D (reversed)
        ADIEncoder{'E', 'F'}  // middle encoder in ADI ports E & F
    )
    // specify the tracking wheels diameter (2.75 in), track (7 in), and TPR (360)
    // specify the middle encoder distance (1 in) and diameter (2.75 in)
    .withOdometry({{2.75_in, 7_in, 1_in, 2.75_in}, quadEncoderTPR})
    .buildOdometry();

//2. Calculate the change in each encoders’ value since the last cycle, and convert to distance of
//wheel travel (for example, if the wheel has a radius 2" and turned 5°, then it travelled
//approximately 0.1745"); call these Δ𝐿, Δ𝑅, and Δ𝑆


//3. Update stored "previous values" of encoders


//4. Calculate the total change in the left and right encoder values since the last reset, and convert
//to distance of wheel travel; call these Δ𝐿𝑟 and Δ𝑅𝑟


//5. Calculate new absolute orientation 𝜃1 = 𝜃𝑟 + Δ𝐿𝑟−Δ𝑅𝑟𝑠𝐿+𝑠𝑅
// please note that the second term will be in radians, regardless of the units of other variables


//6. Calculate the change in angle Δ𝜃 = 𝜃1 − 𝜃0


//7. If Δ𝜃 = 0 (i.e. Δ𝐿 = Δ𝑅), then calculate the local offset Δ𝑑𝑙 = [ Δ𝑆Δ𝑅]


//8. Otherwise, calculate the local offset Δ𝑑𝑙 = 2 sin 𝜃2× [Δ𝑆Δ𝜃 + 𝑠𝑆Δ𝑅Δ𝜃+ 𝑠𝑅] (Equation 6)


//9. Calculate the average orientation 𝜃𝑚 = 𝜃0 +Δ𝜃2


//10. Calculate global offset Δ𝑑 as Δ𝑑𝑙 rotated by −𝜃𝑚; this can be done by converting your existing


//Cartesian coordinates to polar coordinates, changing the angle, then converting back


//11. Calculate new absolute position 𝑑1= 𝑑0 + Δ







std::shared_ptr<OdomChassisController> 
  chassis = ChassisControllerBuilder()
    .withMotors({leftFront, leftTop, leftBottom}, {rightFront, rightTop, rightBottom}) 
    // green gearset, 4 inch wheel diameter, 11.5 inch wheel track
    .withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
    // left encoder in ADI ports A & B, right encoder in ADI ports C & D (reversed)
    .withSensors(
        ADIEncoder{encoderLPort1, encoderLPort2}, // Left encoder
        ADIEncoder{encoderRPort1, encoderRPort2},  // Right encoder
        ADIEncoder{encoderCPort1, encoderCPort2, true}  // Center encoder reversed
   )
    // specify the tracking wheels diameter (2.75 in), track (7 in), and TPR (360)
    .withOdometry({{2.75_in, 7.5_in, 1_in, 2.75_in}, quadEncoderTPR})
    .buildOdometry();
  


void updateAuton()
{
// set the state to zero
//chassis->setState({0_in, 0_in, 0_deg});


//chassis->setMaxVelocity(600);
chassis->driveToPoint ({1_ft, 1_ft});




}

*/
//double difference;
void test()
{

imu.reset();
  driveForward(24);
  //chassis -> driveToPoint({24_in, 24_in}, false);
  //chassis ->driveToPoint({0_in, 24_in}, false);

}

