#include <Servo.h>

Servo myservo;

int default_r, default_g, default_b;
int servo_base_angle = 91;

bool initiator(){ // Aναγνώριση εκίνησης μέτρησης

  int tt = analogRead(0);

  if(tt > (default_r + 7)) {  
    
      Serial.println(String(tt) + " " + String(default_r + 7) + " " );
      return true;  
  }
  return false;
}

char recognition(){

  bool measuring = true;
  int tmp_r = analogRead(0); // διαβάζω τιμή από το Α0 για το κόκκινο
  delayMicroseconds(1);
  int tmp_g = analogRead(1); // διαβάζω τιμή από το Α1 για το πράσινο
  delayMicroseconds(1);
  int tmp_b = analogRead(2); // διαβάζω τιμή από το Α2 για το μπλε

  default_g = tmp_g * 1.1;
  default_b = tmp_b;
  
  int max_r = default_r;
  int max_g = default_g;
  int max_b = default_b;

  while(measuring){    // Μέτρηση
    
    if(tmp_r > max_r)  // Σύγκριση τιμής που διαβάστηκε με την default του κόκκινου και αποθήκευση αυτής
      max_r = tmp_r;

    if(tmp_g * 1.1 > max_g)  // Σύγκριση τιμής που διαβάστηκε με την default του πράσινου και αποθήκευση αυτής
      max_g = tmp_g * 1.1;

    if(tmp_b > max_b)  // Σύγκριση τιμής που διαβάστηκε με την default του μπλε και αποθήκευση αυτής
      max_b = tmp_b;
    
    if((((((max_b - default_b) / 2) + default_b)) > tmp_b) && (max_b > default_b + 4)){// Γενικά αν η τιμή του μπλε αρχιζει να κατεβαίνει, σταματάει η μέτρηση και ξεκινάει απο την αρχή 

      default_r = tmp_r;
      Serial.println(String(default_r) + " " + String(default_g) + " " + String(default_b));
      measuring = false;
    }
    else{
      
      Serial.println(String(max_r - default_r) + " " + String(max_g - default_g) + " " + String(max_b - default_b));
      tmp_r = analogRead(0); // διαβάζω τιμή από το Α0 για το κόκκινο
      delayMicroseconds(1);
      tmp_g = analogRead(1); // διαβάζω τιμή από το Α1 για το πράσινο
      delayMicroseconds(1);
      tmp_b = analogRead(2); // διαβάζω τιμή από το Α2 για το μπλε
    }
  }

  int red = (max_r - default_r);
  int green = (max_g - default_g);
  int blue = (max_b - default_b);

  if((red > green) && (red > blue))// Εύρεση μεγαλύτερης μετρούμενης τιμής και επιστροφή χαρακτήρα που υποδεικνύει το χρώμα 
    return 'r';
  else if((green > red) && (green >= blue))
    return 'g';
  else
    return 'b';
}

void action(char c){

  if(c == 'r')
      myservo.write(servo_base_angle - 30); // red - left position
  else if(c == 'g')
      myservo.write(servo_base_angle);   // green - middle position
  else
      myservo.write(servo_base_angle + 30);   // blue - right+-* position
}

void setup() {

  Serial.begin(9600);

  myservo.attach(9);
  delay(200);

  default_r = analogRead(0) - 1;
  default_g = analogRead(1) - 1;
  default_b = analogRead(2) - 1;
  delay(200);

  myservo.write(servo_base_angle + 15);
  delay(200);
}

void loop() {

  while(!initiator()){}
  char recog = recognition();
  action(recog);
  delay(500);
}
