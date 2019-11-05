// *******************************************************************************************************
// *******************************************************************************************************
// ************  PROGRAMA LUCHA SUMO DE IGNACIO OTERO CONCURSO OSHWDEM 2019 CORUÑA           *************
// *******************************************************************************************************
// *******************************************************************************************************
// ************       NOMBRE PROPUESTO DEL BICHO OMUS               modificaciones 2018      *************
// *******************************************************************************************************
// *******************************************************************************************************

// Idea original de Pololu, adaptado por Ignacio Otero
// Versión del 2 de noviembre, doble pitido al encender y escaneo US tres veces por segundo
// Partimos de la version del 17/2/2016 que funcionaba bien.
// Velocidad, siempre a la máxima
// con excepción de la marcha atrás.
// Version 15/10/2018, cambiamos placa US y pasamos el disparo a A1
// Cambio motores por otros más rápidos de José Miguel
// 2/11/2018 incorporo microservo para manejar muleta,controlado por 6PWM
// 3/11/2018 Cambiar la alimentación del serrvo a Vin, se cuelga con 5vcc
// 9/11/2018 Elimino el movimiento de muleta en la detección y le meto 64gr de plomo
// y amplio el tiempo de giro a 700ms

// Comentario del día siguiente: 11/11/2018
// Se me olvidó el sensor de líneas en el coche!
// No me di cuenta hasta el segundo combate
// En cualquier caso funionó bien sin cosas raras, salvo que se salía por la borda
// Hubiese quedado en un honrroso 3er puesto de tener sensor.
// Hubiese quedado cuarto de no arrancar el el último combate de espaldas.
// Propongo volver con el armazón más sujeto, cambiar el detector de ultirasonidos por Pololu U32A

// IMPORTANTE DESCONECTAR PUENTES EN PLACA ZUMO PARA SENSORES PROXIMIDAD
// Liberamos lector de batería, A1
// Desconexión giroscopo y acelerometro, liberamos pines A4 y A5, los usamos para ultrasonidos
// Esta es la version ultrasonidos 13 y A1, los usamos para ultrasonidos
// Es necesaria una versión distinta para detectores ópticos
// El 6 queda disponible PWM para servo

// PINES USADOS EN ESTA VERSIÓN (15)
// Motores Zumo (4) = 7, 8, 9, 10
// Sensor IR Zumo (7) = A0, A2, A3, 2, 4, 5, 11
// Buzzer Zumo (1) = 3
// Pulsador Pushbutton Zumo (1) = 12
// Ultrasonidos disparo (1) = 13 (led)
// Ultrasonidos Echo centro, izquirda y derecha (3) A1, A4, A5
// Servo a través de pin (1) = 6
// ***********************************************************************************

// CAMBIOS 2019
// Pegar carcasa superior
// Pasar todos los define a configuración
// Poner un estado de arrancada, de frente, de lado y hacia atrás
// Desconectar sensores IR mientras se maneja el servo en la arrancada
// Durante un periodo de tiempo, lo que tarda en bajar el servo:
//       - Posición de frente. Arrancar a máxima velocidad y bajar el servo
//       - Posición de lado. Girar un tiempo, bajar el servo y adelante a máxima
//       - Posición hacia atrás. Girar más tiempo, bajar el servo y a máxima
// A partir de aqui el servo no se toca y se maneja la librería IR ZumoReflectanceSensorArray

/* Este ejemplo utiliza el sensor de bordes del Zumo y su librería para evitar salir del área circular
 * del Dohyo. Además utiliza las librerías de ZumoMotors, PushButton, ZumoBuzzer y Ultrasonic
 *
 *
 * Este programa tiene las siguientes rutinas:
 *
 *  - utiliza el PushButton para comenzar la partida, cuenta atrás y detener el robor
 * 
 *  - utiliza el ZumoBuzzer para tocar efectos de sonido en la competición
 *  
 *  - utiliza QTRSensors y ZumoReflectanceSensorArray para leer los IR de línea
 *  
 *  - utiliza Ultrasonic para leer los sensores de ultrasonidos
 *
 *  - tiene un aleatorizador de tiempo para los giros, evitando seguir un patrón
 *
 */



// LIBRERÍAS
#include "Configuracion.h" // En esta pestaña están los parámetros configurables
#include <ZumoBuzzer.h> // Para las melodías y pitidos
#include <ZumoMotors.h> // Controla los motores
#include <Pushbutton.h> // Pulsador de inicio y parada
#include <QTRSensors.h> // Necesita esta librería para la libreria del sensor de reflectancia
#include <ZumoReflectanceSensorArray.h> // Sensores delanteros de línea
#include <Ultrasonic.h> // Para la versión del detector US
#include <Servo.h>      // Para manejar la "muleta" de engaño y bajar la balda

 
// Definimos una clase de ultrasonidos derecha(Trig Pin, Echo Pin)
Ultrasonic detectorUS(GATILLO, sensor_US_centro); 
Ultrasonic detectorIZ(GATILLO, sensor_US_izquierda); 
Ultrasonic detectorDR(GATILLO, sensor_US_derecha); 

// Definimos una clase de boton del Zumo
Pushbutton pulsador(ZUMO_BUTTON); // En la librería Pushbutton ya se define el pin ZUMO_BUTTON 

// Definimos una clase de sernsor reflectante frontal, sin pin de emisión, no apaga los IR Leds
ZumoReflectanceSensorArray sensorIR(QTR_NO_EMITTER_PIN); 

// Definimos una clase de Zumo Motors 
ZumoMotors motores;

// Definimos una clase de Zumo Buzzer para el sonido
ZumoBuzzer zumbador;

// Definimos una clase de servo para la muleta
Servo muleta; 

// Variables generales
unsigned int valores_sensor[NUM_SENSORES]; // Un array de 6 valores enteros sin signo

int Pos_Capote=Capote_AR; //Es la posición del Capote, inicialmente a 90

// EFECTOS DE SONIDO
const char efecto_sonido[] PROGMEM = "O4 T100 V12 L4 MS g12>c12>e12>G6>E12 ML>G2"; // Melodía de "A la carga..."
 // El sonido se desactiva con V0, para suprimirlo y V15 para el máximo volumen
 
// Esta variable la utilizamos en el loop principal y varias funciones
int direccion = RECTO;
boolean detectado =false;
int distancia=80;
int distancia_IZ=0;
int distancia_DR=0;
unsigned long TiempoGiro=0;
unsigned long PeriodoServo=0; // Intervalo de tiempo para controlar los 20ms del periodo del servo



// CONFIGURACION INICIAL
void setup()
   {  
  
       Serial.begin(57600); // Iniciamos puerto serie pero no se utiliza
       Serial.println("Inicializando.....");
       Serial.println("Pulsa el boton de inicio:?");
       zumbador.playNote(NOTE_G(3), 50, 12); // Toca nota
       delay(250);
       zumbador.playNote(NOTE_G(3), 50, 12); // Toca nota

       // Crea una semilla para hacer el aleatorizador
       randomSeed((unsigned int) millis());
  
       // Comentar las siguiente líneas si es necesario cambiar el sentido de giro de los motores
       //motores.flipLeftMotor(true);
       //motores.flipRightMotor(true);

       // Colocamos la muleta en el pin 6 PWM
       muleta.attach(PinMuleta);

       // La muleta debe estar hacia arriba al empezar
       muleta.write(Capote_AR);
    

       pinMode(LED, OUTPUT); // Ponemos el pin 13 del LED y del gatillo como salida
       
       zumbador.playMode(PLAY_AUTOMATIC); // El sonido está en modo automático
       
       // Espera inicialmente a que se pulse el botón
       PulsarCuentaAtras(false); // Y esperamos a que se toque el botón
   }



// BUCLE PRINCIPAL
void loop()
{

  // Cuando se pulsa por segunda vez en competición, para el robot para reiniciar
  if (pulsador.isPressed()) 
     {
        motores.setSpeeds(0, 0); // Paramos motor
        pulsador.waitForRelease(); // Espera a que se suelte el botón
        // Espera de nuevo a que se pulse el botón para iniciar otra ronda
        PulsarCuentaAtras(true); // Hace la cuenta atrás inicial
     }
  
   sensorIR.read(valores_sensor); // Leemos el array de sensores
   //direccion=RECTO;     // Por defecto en cada bucle la direccion es RECTO
  
  // COMPRUEBA QUE TOCAMOS EL BORDE PARA CAMBIAR DE SENTIDO, CON FONDO NEGRO Y LINEAS BLANCAS
  if (valores_sensor[0] < NIVEL_DISPARO_QTR) // Comprobamos que el sensor de izquierda está en la línea blanca
     { // Si el sensor de la izquierda detecta la línea blanca, entonces retrocede un poco y gira a la derecha
        direccion=DERECHA;
        giro(direccion, true); // La función giro incluye la marcha atrás y despues giro
        busca(direccion);
     }
     
  else if (valores_sensor[5] < NIVEL_DISPARO_QTR) // Ahora comprobamos el de la derecha, que no esté en la línea
     {      // Si el sensor de la derecha detecta la línea blanca, entonces retrocede un poco y gira a la izquierda
       direccion=IZQUIERDA;
       giro(direccion, true); // Giro izquieda, antes un poco atrás
       busca(direccion);
     }

  // Cuatro veces por segundo leemos sensores US y tomamos decisión
  if(millis()-TiempoGiro>330)
    {
        //direccion=RECTO; // Por defecto Recto
        direccion=DimeDireccion(); // La función mira a través de ultrasonidos y devuelve la dirección del objeto
        TiempoGiro=millis();
    }
  
  // LLamamos a ls funcion de busqueda por US, en funcion de la dirección
  // Se encarga de mover los motores en esa direccion
  busca(direccion); // Y si detecta algo de frente empuja recto siempre
        
  } // Fin del bucle principal




// FUNCIÓN PULSAR Y CUETA ATRÁS
// Esta función cuenta hasta 5 segundos, despues de pulsar el boton
void PulsarCuentaAtras(bool restarting)
   { 

     digitalWrite(LED, HIGH); // Activa el LED para avisar que está esperando
     pulsador.waitForButton();  // a que se pulse el boton
     digitalWrite(LED, LOW);  // Una vez pulsado apaga el LED
   
     // Cuenta hasta 3, 3 segundos
     for (int i = 0; i < 3; i++)
      {
        delay(1000); // Un segundo
        zumbador.playNote(NOTE_G(3), 50, 12); // Toca nota
        Serial.println(5-i);
      }

    delay(1000); // Otro segundo
    Serial.println("2   Preparados----!");
    zumbador.playFromProgramSpace(efecto_sonido); // Sonido de a la carga
    delay(1000); // Otro segundo, en total 5.
    Serial.print("1    ");
    Serial.println("A LA CARGA....!");
    
  }





// FUNCION GIRO, ESTA FUNCIÓN SE ACTIVA AL TOCAR LOS BORDES
// IDICAR LA DIRECCIÓN DERECHA O IZQUIERDA
// RAMDOMIZE es para mejorar la búsqueda con tiempo aleatorio.
void giro(int direction, bool semilla_ramdom)
   {
  
     static unsigned int incremento_tiempo = TIEMPO_GIRO / 4; // Unos 35ms

     motores.setSpeeds(-RETROCESO, -RETROCESO); // Marcha atrás
     delay(TIEMPO_RETROCESO);                   // Retrocede 200ms
  
     motores.setSpeeds(AVANCE * direction, -AVANCE * direction); // Gira derecha o irzquierda
     delay(semilla_ramdom ? TIEMPO_GIRO + (random(8) - 2) * incremento_tiempo : TIEMPO_GIRO); // El tiempo de giro es aleatorio
 
     motores.setSpeeds(AVANCE, AVANCE);

   }





// FUNCION BUSCA CON SENSOR ULTRASONIDOS
// Busca enemigo en un sentido derecha o izquierda
// Busca y Gira hasta que encuentra un objeto
// Se puede mejorar para que avance un poco y haga círculos
void busca(int direction)
  {
    // Avanzamos si es recto, en caso de izq o der, giramos hasta encontrar algo
    // Como mucho 600ms, despues salimos avanzando de nuevo
    unsigned long tiempo=0;
    tiempo=millis(); // Tomamos nota del tiempo para que no nos pasemos de 0,8 segundos
    int distancia=80;
   
    if(direction!=RECTO) // En caso de que NO busquemos hacia adelante
          { // Mientras la distancia es más de 50 y no superamos 1 segundo
            while((distancia=detectorUS.Ranging(CM)>ALCANCE)&&(millis()-tiempo<TIEMPO_BUSCA)) // Antes giraba hasta 1200
               { // GIRAMOS HASTA ENCONTRAR ALGO O COMO MÁXIMO 1,0 SEGUNDOS
                 motores.setSpeeds(AVANCE * direction, -AVANCE * direction);
               }
           }
     motores.setSpeeds(AVANCE, AVANCE); // Salimos avanzando de la busqueda

  }// Fin de la funcion


// FUNCION DIME DIRECCION
// DEVUELVE LA DIRECCION EN FUNCION DE LA LECTURA DE LOS SENSORES US
int DimeDireccion()
{
     // Leemos los tres sensores para ver si detecta algo
     distancia=detectorUS.Ranging(CM);
     delay(10); // Esperamos 10 ms para evitar ecos
     distancia_IZ=detectorIZ.Ranging(CM);
     delay(10); // Esperamos 10 ms para evitar ecos
     distancia_DR=detectorDR.Ranging(CM);
     delay(10); // Esperamos 10 ms para evitar ecos

     if (distancia<=ALCANCE) 
        {
          Serial.print("Objetivo RECTO cm ");
          Serial.println(distancia);
          return RECTO; // Si está defrente devuelve Recto
        }        
     if (distancia_IZ<=ALCANCE) 
        { 
          Serial.print("Objetivo IZQUIERDA cm ");
          Serial.println(distancia_IZ);
          return IZQUIERDA; // Si está a la izquierda devuelve Izquierda
        }
     if (distancia_DR<=ALCANCE) 
        {
          Serial.print("Objetivo DERECHA cm ");
          Serial.println(distancia_DR);
          return DERECHA; // Si está a la derecha devuelve Derecha
        }
     if ((distancia>ALCANCE)&&(distancia_IZ>ALCANCE)&&(distancia_DR>ALCANCE)) 
        { 
          Serial.println("Sin Objetivo"); // Puede estar detrás
          return RECTO; // cambiar por un giro
        }
        
  }
    
