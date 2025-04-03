#include <vehicle.h>

int Speeds = 255;

vehicle myVeh;

void handlemoveCar() {  //Smart car control functions
  String move = "a";
  if (move == "f") {
    myVeh.Move(Forward, Speeds);
  } else if (move == "b") {
    myVeh.Move(Backward, Speeds);
  } else if (move == "l") {
    myVeh.Move(Contrarotate, Speeds);
  } else if (move == "r") {
    myVeh.Move(Clockwise, Speeds);
  } else if (move == "s") {
    myVeh.Move(Stop, 0);
  } else if (move == "tl") {
    myVeh.Move(Move_Left, Speeds);
  } else if (move == "tr") {
    myVeh.Move(Move_Right, Speeds);
  }
}

void setup() {
  myVeh.Init();
  myVeh.Move(Stop, 0);
}
void loop() {
  myVeh.Move(Forward, Speeds);
}