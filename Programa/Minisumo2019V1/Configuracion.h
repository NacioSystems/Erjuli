

// PINES FIJOS QUE NO ESTÁN EN LIBRERÍAS
#define LED 13     // Lo usamos para led
#define GATILLO 13 // Es el mismo que el led y los usamos para el disparo de US

#define sensor_US_izquierda A4 // Sensor de ultrasonidos de la izquierda
#define sensor_US_derecha   A5 // Sensor de ultrasonidos de la derecha
#define sensor_US_centro    A1 // Sensor de ultrasonidos central

// PARÁMETROS DEL SENSOR REFLECTANTE IR FRONTAL
#define NUM_SENSORES 6 // Es el número de sensores a utilizar en el sensor IR, todos
#define NIVEL_DISPARO_QTR  300 // Es el valor de decisión, en microsegundos, tenía anteriormente 300 para borde blanco, 900 cartón marrón borde negro
                               // Hay que pensar que el Dohyo es negro y la línea blanca


// PARÁMETROS DEL MOTOR, DEBEN AJUSTARSE EN FUNCIÓN DEL MOTOR
// Velocidades
#define RETROCESO         200 // 0 parado, 400 máxima velocidad
#define AVANCE            400
#define TIEMPO_RETROCESO  400 // Con el anterior motor estaba a 200 ms
#define TIEMPO_GIRO       150 // Con el anterior motor estaba a 150 ms (600)
#define TIEMPO_BUSCA     1000 // Tenía 700

// Giros
#define DERECHA 1        // Multiplica por la velocidad por eso cambia de signo
#define IZQUIERDA -1
#define RECTO 0
#define CURVA 2          // No se utiliza

// Distancia de alcance en cm
#define ALCANCE 49

// Muleta
#define PinMuleta 6
#define Capote_DR 50
#define Capote_IZ 140
#define Capote_AR 90
