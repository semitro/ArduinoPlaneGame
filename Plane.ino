#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
class GameObject{
  public:
  enum Direction{
    RIGHT,
    LEFT,
    UP,
    DOWN
  };
  struct Position{
    int x,y = 0;
  };
  
  protected:
  LiquidCrystal *lcd;
  // Это массив!
  byte *pixMap;
  // Рисовка нижнего символа, чтобы можно было перелетать по частям экрана
  byte downMap[8] = {
   B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  };
  
  Position pos;
  // Приходится помнить, чтобы очищать старое изображение
  // метод getCursor не наблюдается 
  Position prevPos;
  public:
    GameObject(const LiquidCrystal *lcd){
      this->lcd = lcd;
 
    }
    // Передаваемый параметр - массив!
  void setPixMap(byte *pixMap){
    this->pixMap = pixMap;
  }
  void render(){
    lcd->createChar(0,pixMap);
    lcd->createChar(1,downMap);
    lcd->setCursor(prevPos.x, prevPos.y);
    lcd->print(" ");
   
    lcd->setCursor(pos.x,pos.y);
    lcd->write(byte(0));

    lcd->setCursor(prevPos.x,1);
    lcd->print(" ");
    lcd->setCursor(pos.x,1);
    lcd->write(byte(1));
  }
  void move(Direction dir){
    prevPos = pos;
    if(dir == RIGHT) pos.x++;
    if(dir == LEFT)  pos.x--;
    if(dir == UP)    pos.y++;
    if(dir == DOWN)  this->moveDown();
  }
  private:
  void moveDown(){
    downMap[0] = pixMap[7];
    for(int k(7);k>=1;k--){
        pixMap[k] = pixMap[k-1];
        downMap[k] = downMap[k-1];
    }
  }
};

class Plane : public GameObject{
  public:
  static byte planeMap[8];
  public:
  Plane(LiquidCrystal *lcd): GameObject(lcd){
    this->pixMap = planeMap;
  }
};
byte Plane::planeMap[8] = {
  B00000,
  B00010,
  B10010,
  B11111,
  B10010,
  B00010,
  B00000,
  };
 
Plane plane(&lcd);

void setup() {
  // set up the LCD's number of columns and rows:
   // lcd.noAutoscroll();
  pinMode(6, OUTPUT);
  analogWrite(6, 2);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  // Print a message to the LCD.
  plane.render();
}

void loop() {
  // put your main code here, to run repeatedly:
    
    delay(1000);
    plane.move(GameObject::DOWN);
    plane.move(GameObject::RIGHT);
    plane.render();

}
