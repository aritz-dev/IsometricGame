PUNTOS A CORREGIR:

- esta en segunda persona, incorrecto, que sea impersonal.
- sigue habiendo emoticonos
- las formulas fisicas de la vida real no estan bien representadas, deben ser asi asi:
$$
x_{\text{new}} = x_{\text{old}} + d_x \cdot v \cdot \Delta t
$$ 
- conceptos de la fisica deben ir junto a la solucion algo como "nosotros lo que realmente queremos hacer es esta formula para que se lo mas parecido a fisicas reales... bueno pues esto plasmado en los codigos seria asi.."
- apartados como 10. RESUMEN Y BUENAS PRÁCTICAS y conclusiones y resumenes sobran...
- se mencionan cosas como grvedad... no hace falta, ya que no aporta anda nuevo a lo fundamental...
- no uses palabras complejas o refinadas, y tampoco frases cortas... osea las explicaciones deben ser claros y faciles de leer y entender.
# FUNDAMENTOS DE CONTROL DE TIEMPO EN VIDEOJUEGOS
Al desarrollar un videojuego, uno de los primeros problemas que aparecen es el movimiento de objetos en pantalla. Este documento explica cómo hacer que el juego funcione a la misma velocidad en cualquier equipo, independientemente de que sea un PC muy potente o un portátil modesto.
​

El objetivo es mostrar por qué un código que parece correcto puede producir velocidades diferentes según el hardware y cómo corregirlo mediante técnicas estándar de la industria, basadas en el uso de medidas de tiempo reales.


## 1. EL CÓDIGO INICIAL

En este apartado se presenta la solución más habitual cuando se intenta mover algo por primera vez. Es el código más directo, el que se suele escribir de manera intuitiva, y sirve para ver por qué, aunque parezca funcionar, en realidad es incorrecto.

El planteamiento inicial suele ser: "cada vez que se actualice el juego, el objeto se mueve un poco". El código podría tener esta forma:


```cpp
float speed = 0.1f;
int directionX = 0;  // -1 izquierda, 0 quieto, 1 derecha
int directionY = 0;  // -1 arriba, 0 quieto, 1 abajo

// Dentro del game loop
rect.x += speed * directionX;
rect.y += speed * directionY;
```

La lógica es sencilla: en cada iteración del bucle principal, el rectángulo avanza 0.1 píxeles en la dirección indicada. 

Tras compilar y ejecutar en el ordenador, el movimiento se percibe fluido. Sin embargo, este comportamiento es engañoso y se explica mas detalladamente el por que en el siguiente apartado.

## 2. PRUEBA EN DIFERENTES CONDICIONES DE HARDWARE
Se puede probar de esta manera: si se tiene un portátil, se desconecta el cargador. Los portátiles, cuando están sin cargador, reducen la potencia del procesador para ahorrar batería. Esto es automático, el sistema operativo lo hace solo.

Al correr el código de nuevo, se observa que el objeto a mover desplaza más lento. Si se conecta el cargador de nuevo, el cuadrado vuelve a ir rápido. Cada jugador va a tener una experiencia diferente dependiendo de su PC.

El problema descubierto es: la velocidad del juego depende del hardware, no del código. Para arreglarlo, se necesita entender qué está pasando por dentro. La respuesta está en cómo funciona el game loop y cómo el procesador ejecuta el programa.

## 3. ANATOMÍA DEL GAME LOOP

En este apartado se analiza qué pasa realmente cuando el juego corre. Entender esto es clave para resolver el problema.

### 3.1 Estructura básica del ciclo principal

Un videojuego es básicamente un bucle infinito que se repite todo el rato:

```cpp
while (running) {
    HandleEvents();  // 1. Leer teclado, mouse, etc.
    Update();        // 2. Actualizar posiciones, lógica (AQUÍ está nuestro código)
    Render();        // 3. Dibujar todo en pantalla
}
```

Cada vuelta completa de este bucle equivale a un frame (un fotograma) del juego. Es como una película: cada frame es una imagen. Cuantos más frames por segundo, más fluido se ve.

### 3.2 Velocidad de iteración del loop

¿A qué velocidad da vueltas este bucle? La velocidad depende de qué tan rápido es el procesador (GHz, núcleos). Cada vuelta del game loop necesita que el procesador ejecute millones de instrucciones (todas las líneas de código de HandleEvents, Update, y Render).

Los procesadores tienen un reloj interno, como un metrónomo. La velocidad de ese reloj se mide en GHz (Gigahercios): 1 GHz = 1,000,000,000 tics por segundo.

Si el procesador hace más tics por segundo: completa cada vuelta más rápido, hay más vueltas por segundo, se ejecuta más veces la línea de actualización de posición, y el objeto se mueve más rápido.

Por eso el juego va más lento sin el cargador. El procesador da menos vueltas, entonces el código se ejecuta menos veces por segundo.

## 4. CONCEPTOS FUNDAMENTALES DE MEDICIÓN

Antes de resolver el problema, se necesita aprender a medir y hablar sobre tiempo. Estos son los términos que usa la industria.

### 4.1 Frames Per Second (FPS)

Indica cuántas vueltas del loop hace el juego en 1 segundo. Es lo mismo que "frecuencia" en física: ciclos por segundo.

60 FPS: el loop se ejecuta 60 veces por segundo

### 4.2 Frame Time (Teórico)

Duración ideal que debería tardar 1 vuelta completa del loop (sin variaciones reales).
$$
\text{Frame Time} = \frac{1}{\text{FPS}}
$$

| FPS | Cálculo | Frame Time |
|-----|---------|------------|
| 30 | 1 ÷ 30 | 0.0333 seg = 33.3 ms |
| 60 | 1 ÷ 60 | 0.0166 seg = 16.6 ms |
| 144 | 1 ÷ 144 | 0.0069 seg = 6.9 ms |

### 4.3 Delta Time (Real - Medido)

Mide el tiempo real transcurrido entre inicio del frame actual e inicio del frame anterior. No mide duración interna de este loop (principio-fin del mismo frame).

De esta manera captura todo el intervalo entre iteraciones, tomando en cuenta llamada al prinpicio del bucle y otros procesos que se ejecutan entre loops.
```cpp
double lastTime = 1.000;  // ← INICIO frame ANTERIOR

// ============= INICIO frame ACTUAL =============
double currentTime = 1.016;  // ← Momento real ahora
double dt = currentTime - lastTime;  // dt = 16 ms TOTAL
lastTime = currentTime;              // Guardar para siguiente

// Procesamiento (~10 ms) + espera externa (~6 ms)
// ============= FIN frame ACTUAL =============
```

## 5. ANÁLISIS Y DIAGNOSTICO DEL PROBLEMA

Regresando al código problemático:

rect.x += 0.1;  // Se ejecuta en cada vuelta del loop

El problema es que se está diciendo la velocidad en "pixels por vuelta" cuando se debería decir en "pixels por segundo".

Si se dijera "voy a correr 0.1 pasos" sin decir "por minuto" o "por hora", no tiene sentido. Lo mismo pasa aquí. Se necesita expresar velocidades en tiempo real, no en "vueltas del programa".

## 6. SOLUCIÓN

La solución es plasmar la física del mundo real en el código. Usamos el Movimiento Rectilíneo Uniforme (MRU), que describe un objeto moviéndose a velocidad constante.

### 6.1 Fórmula del MRU

En física real:
$$
x_{\text{nueva}}(m) = x_0(m) + v\left(\frac{m}{s}\right) \cdot \Delta t(s)
$$

En videojuegos (mismos conceptos, unidades en píxeles):
$$
x_{\text{nueva}}(pixel) = x_0(pixel) + v\left(\frac{pixel}{seg}\right) \cdot \Delta t(s)
$$

**Delta Time (\(\Delta t\))** multiplica la velocidad por el **tiempo real transcurrido**, no por "vueltas del loop".

```cpp
float velocity = 100.0f;  // 100 píxeles/segundo
x = x + velocity * deltaTime;
```

### 6.2 Demostración: Delta Time compensa FPS

**PC rápido (240 FPS, dt = 4.2 ms = 0.0042s):**
```
Frame 1: x += 100 × 0.0042 = 0.42 píxeles
Frame 2: x += 100 × 0.0042 = 0.42 píxeles  
...
Frame 240: x += 100 × 0.0042 = 0.42 píxeles
TOTAL: 0.42 × 240 = 100.8 píxeles ≈ 100 píxeles/segundo
```

**PC normal (60 FPS, dt = 16.7 ms = 0.0167s):**
```
Frame 1: x += 100 × 0.0167 = 1.67 píxeles
Frame 2: x += 100 × 0.0167 = 1.67 píxeles
...
Frame 60: x += 100 × 0.0167 = 1.67 píxeles
TOTAL: 1.67 × 60 = 100.2 píxeles ≈ 100 píxeles/segundo
```
### 6.3 Verificación
**Ambos PCs avanzan 100 píxeles en 1 segundo exacto.** Delta Time hace que menos frames (PC lento) compensen con saltos más grandes, y más frames (PC rápido) con saltos más pequeños. Velocidad **idéntica** en cualquier hardware.

## 7. IMPLEMENTACIÓN PRÁCTICA

Los sistemas operativos tienen contadores de alta precisión. SDL3 ofrece dos funciones:

**`SDL_GetPerformanceCounter()`**
- Devuelve el valor actual del contador (un número grande)
- Son "ticks" del reloj del sistema

**`SDL_GetPerformanceFrequency()`**
- Indica cuántos ticks hay en 1 segundo
- Por ejemplo: 10,000,000 ticks/segundo

Para convertir ticks a segundos: segundos = ticks / frecuencia

### 7.2 Ejemplo completo mínimo

Así se implementa en código real:

```cpp
// Variables de estado
float x = 0.0f;
float velocity = 200.0f;  // px/segundo

// Inicialización del tiempo
Uint64 lastTime = SDL_GetPerformanceCounter();
double frequency = SDL_GetPerformanceFrequency();

while (running) {
    // 1. Medir deltaTime
    Uint64 now = SDL_GetPerformanceCounter();
    float deltaTime = (float)((now - lastTime) / frequency);
    lastTime = now;
    
    // 2. Procesar input
    const bool* keys = SDL_GetKeyboardState(NULL);
    int dx = keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A];
    
    // 3. Update con deltaTime
    x += velocity * dx * deltaTime;
    
    // 4. Render 
    // ...
}
```

## 8. CASOS ESPECIALES

Ahora se van a ver algunos casos comunes que se van a encontrar.

### 8.1 Normalización de vectores de dirección

Hay un problema sutil en el código. Cuando se presionan dos teclas a la vez (por ejemplo W y D para ir en diagonal), el objeto va más rápido.

¿Por qué?


- Una dirección: (1, 0) longitud = 1
- Diagonal: (1, 1) longitud = $\sqrt{1^2 + 1^2}$ = $\sqrt{2}$ ≈ 1.41

El vector diagonal es aproximadamente 41% más largo, entonces el objeto va 41% más rápido en diagonal. Eso no tiene sentido.

Solución: normalizar el vector (hacerlo de longitud 1):
```cpp
float length = sqrt(dx*dx + dy*dy);
if (length > 0) {
    dx = dx / length;
    dy = dy / length;
}

x += velocity * dx * deltaTime;
y += velocity * dy * deltaTime;
```
Ahora el movimiento diagonal va a la misma velocidad que el recto.      

### 8.2 Medición de FPS actuales

Para ver los FPS en pantalla (útil para debug):

**Método simple:**
```cpp
float fps = 1.0f / deltaTime;
printf("FPS: %.1f\n", fps);
```

Problema: el número salta mucho, es difícil de leer.

**Método recomendado (promedio):**
```cpp
const int SAMPLE_SIZE = 60;
float frameTimes[SAMPLE_SIZE];
int frameIndex = 0;

// Cada frame
frameTimes[frameIndex] = deltaTime;
frameIndex = (frameIndex + 1) % SAMPLE_SIZE;

// Calcular promedio
float sum = 0.0f;
for (int i = 0; i < SAMPLE_SIZE; i++) {
    sum += frameTimes[i];
}
float avgFPS = SAMPLE_SIZE / sum;

// Mostrar cada medio segundo (para no spam)
static float printTimer = 0.0f;
printTimer += deltaTime;
if (printTimer >= 0.5f) {
    printf("FPS: %.1f\n", avgFPS);
    printTimer = 0.0f;
}
```

Esto te da un número más estable y legible.

### 8.3 Limitación de FPS máximos

A veces se quiere limitar los FPS a un máximo (por ejemplo 60) para ahorrar batería o sincronizar con el monitor.

**Método 1: Pausar manualmente**

```cpp
const float TARGET_FPS = 60.0f;
const float TARGET_FRAME_TIME = 1.0f / TARGET_FPS;  // 0.0166s

// Al final del loop, medir cuánto tardó
Uint64 frameEnd = SDL_GetPerformanceCounter();
float frameTime = (frameEnd - frameStart) / frequency;

// Si se terminó antes de tiempo, pausar
if (frameTime < TARGET_FRAME_TIME) {
    float delayTime = TARGET_FRAME_TIME - frameTime;
    Uint32 delayMS = (Uint32)(delayTime * 1000.0f);
    SDL_Delay(delayMS);
}
```

**Método 2: VSync (más simple)**

```cpp
// Al crear el renderer
SDL_SetRenderVSync(renderer, 1);

// SDL_RenderPresent() esperará automáticamente
// al refresco vertical del monitor (60 Hz o 144 Hz)
SDL_RenderPresent(renderer);
```

VSync es más fácil pero menos flexible (solo sincroniza con el monitor, no se pueden elegir FPS arbitrarios).


## 9. CONCEPTOS AVANZADOS
Esto no es necesario para empezar, pero es importante saberlo cuando el juego crezca.

### 9.1 Fixed Timestep (paso de tiempo fijo)

Fixed Timestep es una técnica donde el Update siempre usa el mismo deltaTime, sin importar cuánto tardó el frame real.

Cómo funciona:
- Se define un deltaTime fijo (por ejemplo: 0.016s = 60 FPS)
- Si un frame tarda más, se ejecuta Update múltiples veces para "alcanzar" el tiempo

Cuándo se usa:
- Juegos multijugador (todos deben estar sincronizados)
- Física compleja (necesita pasos de tiempo consistentes)
- Grabación y replay de partidas

Implementación básica:

```cpp
const float FIXED_DT = 1.0f / 60.0f;  // 16.6ms fijo
float accumulator = 0.0f;

while (running) {
    float frameTime = MeasureRealFrameTime();
    accumulator += frameTime;
    
    // Ejecutar update tantas veces como sea necesario
    while (accumulator >= FIXED_DT) {
        Update(FIXED_DT);  // Siempre usa 0.016s
        accumulator -= FIXED_DT;
    }
    
    Render();
}
```

Ejemplo: Si un frame tarda 32ms:
1. accumulator = 0 + 0.032 = 0.032s
2. Loop 1: 0.032 >= 0.016, Update(0.016), accumulator = 0.016
3. Loop 2: 0.016 >= 0.016, Update(0.016), accumulator = 0
4. Salir, render

Se ejecutó Update dos veces para compensar el frame lento.

Ventajas:
- Determinismo total (mismo input = mismo output siempre)
- Física estable

Desventajas:
- Más complejo
- En frames muy lentos puede ejecutar Update muchas veces

Recomendación: Usar variable deltaTime (lo que se ha aprendido) para juegos singleplayer. Solo usar fixed timestep si se hace multiplayer o se necesita determinismo estricto.

### 9.2 Interpolación de estados

Cuando se usa fixed timestep, puede quedar un "resto" en el accumulator. Esto hace que el renderizado no sea perfectamente fluido. La solución es interpolar entre estados. Es un tema avanzado que no se cubre aquí, pero si se investiga fixed timestep, se va a encontrar con esto.



## 10 ESTRUCTURA DEL CÓDIGO RECOMENDADO

```cpp
// ═══════════════════════════════════════
// INICIALIZACIÓN (fuera del loop)
// ═══════════════════════════════════════
Uint64 lastTime = SDL_GetPerformanceCounter();
double frequency = SDL_GetPerformanceFrequency();

while (running) {
    // ═══════════════════════════════════════
    // 1. MEDIR TIEMPO
    // ═══════════════════════════════════════
    Uint64 now = SDL_GetPerformanceCounter();
    float deltaTime = (float)((now - lastTime) / frequency);
    lastTime = now;
    
    // ═══════════════════════════════════════
    // 2. EVENTOS
    // ═══════════════════════════════════════
    HandleEvents();
    
    // ═══════════════════════════════════════
    // 3. UPDATE (con deltaTime)
    // ═══════════════════════════════════════
    Update(deltaTime);
    
    // ═══════════════════════════════════════
    // 4. RENDER
    // ═══════════════════════════════════════
    Render();
}
```

**Artículos:**
- "Fix Your Timestep" - Glenn Fiedler (búscalo en Google)
- "Game Programming Patterns" - Robert Nystrom (capítulo Game Loop)

**Documentación:**
- SDL3: wiki.libsdl.org
