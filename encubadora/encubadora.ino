/**
 * PROGRAMA DE ENCUBADORA DE HUEVO
 * INTEGRANTES DEL EQUIPO: 
 * CHÁVEZ SÁNCHEZ KEVIN EDILBERTO
 * DIAZ RUIZ ALEJANDRO
 * HERNÁNDEZ SANTIAGO YENY CELESTE
 * LAVARIEGA PACHECO RODRIGO
 * PÉREZ SIBAJA JOSELYNE ANAHÍ
 * 
 */

/**
 * AGREGAMOS LAS LIBRERÍAS DE NUESTROS COMPONENTES
 * DHT --> SENSOR DE TEMPERATURA
 * LIQUID_CRYSTAL --> PANTALLA LCD 16X2
 */
#include <DHT.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define DHTPIN 3        // Pin donde está conectado el sensor
#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(10, 9, 8, 7, 6, 5); //Pines asociados a las salidas

int rojo = 4;
int ventilador = 11;
int zumbador = 12;
int contador = 0;
int girar = 2;

void setup() {
    Serial.begin(9600);  
    
    lcd.begin(16, 2);           // INDICAMOS QUE TENEMOS CONECTADA UNA PANTALLA DE 16X2
    lcd.home();                 // MOVER EL CURSOR A LA PRIMERA POSICION DE LA PANTALLA (0, 0)
    lcd.print("Iniciando...");  // IMPRIMIR MENSAJE EN LA PRIMERA LINEA
    delay(3000);                // ESPERAR 3 SEGUNDOS
    
    dht.begin();                // INDICAMOS QUE TENEMOS CONECTADO UN SENSOR DHT22
    
    pinMode(rojo,OUTPUT);       // SALIDA DEL FOCO
    pinMode(zumbador,OUTPUT);   // ZUMBADOR
    pinMode(ventilador,OUTPUT); // VENTILADOR
    pinMode(girar,OUTPUT);      // MOTOR PARA GIRAR LOS HUEVOS
}
void loop() {
    delay(2000);
    contador = contador + 1;             // CONTADOR PARA MEDIR EL TIEMPO
    float h = dht.readHumidity();        // LEER HUMEDAD
    float t = dht.readTemperature();     // LEER TEMPERATURA EN GRADOS C°
    float f = dht.readTemperature(true); // LEER TEMPERATURA EN GRADOS F°
    //--------Enviamos las lecturas por el puerto serial-------------
    lcd.clear();
    lcd.setCursor(0, 0);                 //POSICIONAR LA IMPRESIÓN EN EL LCD 
    lcd.print("T=");
    lcd.print(t);
    lcd.print("C H=");
    lcd.print(h, 1);
    lcd.print("%");

    /**
     * ÁREA DE VALIDACIÓN DE TEMEPRATURA
     */
    if(t>=38.9){
        lcd.setCursor(0, 1);
        lcd.print("T: MUY ALTA");
        digitalWrite(zumbador,HIGH);
        digitalWrite(ventilador,HIGH);
        digitalWrite(rojo,LOW);
    }else{
        if(t>37.00 && t<=38.8){
            lcd.setCursor(0, 1);
            lcd.print("T: NORMAL");
            digitalWrite(zumbador,LOW);
            digitalWrite(ventilador,LOW);
            digitalWrite(rojo,LOW);
        }else{
            if(t>36.00 && t<=37.00){
                lcd.setCursor(0, 1);
                lcd.print("T: BAJA-A");
                digitalWrite(zumbador,LOW);
                digitalWrite(ventilador,LOW);
                digitalWrite(rojo,HIGH);
            }else{
                if(t<=36.00){
                    lcd.setCursor(0, 1);
                    lcd.print("T: MUY BAJA");
                    digitalWrite(zumbador,HIGH);
                    digitalWrite(ventilador,LOW);
                    digitalWrite(rojo,HIGH);
                }else{
                    digitalWrite(zumbador,LOW);
                    digitalWrite(ventilador,LOW);
                    digitalWrite(rojo,LOW);
                } 
            }
        }
    }

    /**
     * ÁREA DE VALIDACIÓN DE HUMEDAD
     * CUANDO LA SALIDA ES: 
     * H=B --> HUMEDAD BAJA
     * H=A --> HUMEDAD ALTA
     * H=N --> HUMEDAD NORMAL
     */

    if(h < 56.00){
          digitalWrite(ventilador,HIGH);
          lcd.setCursor(13, 1);
          lcd.print("H=B"); //HUMEDAD BAJA
    }else{
        if(h > 60.00){
            digitalWrite(ventilador,LOW);
            lcd.setCursor(13, 1);
            lcd.print("H=A"); //HUMEDAD ALTA
        }else{
             if (h >= 56.00 && h <= 60.00 ){
                  digitalWrite(ventilador,LOW);
                  lcd.setCursor(13, 1);
                  lcd.print("H=N"); //HUMEDAD NORMAL
             }
        }
    }
    
    /** 
     *  ÁREA PARA VERIFICAR EL TIEMPO ADECUADO PARA GIRAR LOS HUEVOS
     *  Cuando el contador es igual a 1, significa que han pasado 1 segundos
     *  
     */
    if (contador == 1800){
        for (int i = 0 ; i < 30 ; i++){
            digitalWrite(girar,HIGH);  
            delay(1000);
            lcd.setCursor(0, 1);
            lcd.print("GIRANDO HUEVOS");
        }
        digitalWrite(girar,LOW);
        contador = 0;  
    }    
}
