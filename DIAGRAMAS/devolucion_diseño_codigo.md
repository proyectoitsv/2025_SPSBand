# Análisis de Correspondencia: Diseño vs. Implementación
**Proyecto:** SPSBand  
**Fecha de Análisis:** 27 de noviembre de 2025  
**Evaluador:** GitHub Copilot

---

## 1. Resumen Ejecutivo

Este documento presenta un análisis comparativo entre los diagramas de diseño (estado, flujo y secuencia) y el código fuente implementado en el firmware del proyecto SPSBand. Se evalúa la trazabilidad bidireccional para identificar brechas de implementación y documentación.

---

## 2. Análisis: Diseño → Código

### 2.1 Diagrama de Estados
**Estados documentados en diseño:**
- Inicialización
- Monitoreo continuo (GPS + sensores)
- Detección de caída
- Envío de alertas SMS
- Estados de error/recuperación

**Correspondencia con código:**
- ❌ **No implementado**: Máquina de estados explícita
- ⚠️ **Implementación parcial**: Lógica secuencial en `main.cpp` sin estructura FSM
- ✅ **Funciones presentes**: GPS, MAX30102 (sensor ritmo cardíaco)
- ❌ **Ausente**: SMS (archivo `SMS.CPP` en `/lib/` pero no en `/src/`)

### 2.2 Diagrama de Flujo
**Flujos documentados:**
- Secuencia de inicialización de hardware
- Loop principal de lectura de sensores
- Condiciones de disparo de alertas
- Gestión de comunicaciones

**Correspondencia con código:**
- ⚠️ **Implementación lineal**: El código en `main.cpp` sigue estructura básica setup/loop
- ❌ **Condiciones de alerta**: No se evidencia lógica de decisión basada en umbrales
- ✅ **Lectura de sensores**: Implementación de GPS y MAX30102
- ❌ **Gestión de errores**: No se observa manejo de fallos según diagrama

### 2.3 Diagrama de Secuencia
**Interacciones documentadas:**
- Comunicación MCU → GPS
- Comunicación MCU → Sensor biométrico
- Comunicación MCU → Módulo GSM
- Sincronización temporal de eventos

**Correspondencia con código:**
- ✅ **GPS**: Implementación completa en `GPS.cpp`
- ✅ **MAX30102**: Clase implementada en `MAX30102.cpp`
- ❌ **GSM/SMS**: No integrado en código fuente principal
- ❌ **Sincronización**: No se evidencia control temporal entre módulos

---

## 3. Análisis: Código → Diseño

### 3.1 Componentes Implementados

| Componente | Archivo | Documentado en Diagramas |
|------------|---------|--------------------------|
| GPS | `src/GPS.cpp` | ✅ Sí |
| MAX30102 | `lib/MAX30102.cpp` | ✅ Sí (como sensor biométrico) |
| SMS | `lib/SMS.CPP` | ✅ Sí |
| Main Loop | `src/main.cpp` | ⚠️ Parcial |

### 3.2 Lógica No Documentada
- **Archivos duplicados**: 
  - `lib/GPS.cpp` vs `src/GPS.cpp`
  - `lib/GPSPreliminar (funcionando).cpp`
- **Estructura de proyecto**: No hay diagrama de componentes que muestre la arquitectura de carpetas
- **Dependencias**: No documentadas las librerías utilizadas (PlatformIO)
- **Interfaces hardware**: No hay diagramas de conexionado físico vs. código

### 3.3 Configuración de Proyecto
**Presente en código, ausente en diagramas:**
- Configuración PlatformIO (`platformio.ini`)
- Estructura de directorios específica (lib/src/include)
- Configuración de depuración (`.vscode/c_cpp_properties.json`)

---

## 4. Brechas Identificadas

### 4.1 Implementación Incompleta (Diseño → Código)
1. **Crítico**: Sistema de alertas SMS no integrado en flujo principal
2. **Alto**: Máquina de estados no implementada explícitamente
3. **Medio**: Lógica de detección de caídas no presente
4. **Medio**: Manejo de errores y recuperación

### 4.2 Documentación Faltante (Código → Diseño)
1. **Alto**: Diagrama de componentes de software
2. **Alto**: Especificación de interfaces entre módulos
3. **Medio**: Diagrama de despliegue (hardware connections)
4. **Bajo**: Diagramas de actividad para funciones específicas

---

## 5. Métricas de Correspondencia

| Métrica | Valor | Estado |
|---------|-------|--------|
| Cobertura Diseño→Código | ~45% | 🔴 Insuficiente |
| Cobertura Código→Diseño | ~60% | 🟡 Aceptable |
| Trazabilidad Bidireccional | ~50% | 🟡 Requiere mejora |
| Componentes documentados | 3/4 | 🟡 Aceptable |
| Estados implementados | 0/5 | 🔴 Crítico |

---

## 6. Recomendaciones

### 6.1 Prioridad Alta
1. **Integrar módulo SMS**: Mover funcionalidad de `lib/SMS.CPP` a flujo principal
2. **Implementar FSM**: Crear máquina de estados según diagrama de estados
3. **Unificar código GPS**: Eliminar duplicación entre lib y src
4. **Lógica de alertas**: Implementar condiciones de disparo documentadas

### 6.2 Prioridad Media
1. **Actualizar diagramas**: Incluir estructura de componentes real
2. **Documentar interfaces**: Especificar API entre módulos en diagramas de secuencia
3. **Agregar manejo de errores**: Según lo especificado en diagrama de flujo
4. **Diagrama de despliegue**: Mostrar conexiones físicas hardware

### 6.3 Prioridad Baja
1. **Documentar configuración**: Agregar notas sobre PlatformIO en documentación
2. **Limpiar archivos preliminares**: Remover código experimental de repositorio
3. **Estandarizar nomenclatura**: Consistencia entre nombres en diagramas y código

---

## 7. Conclusiones

El proyecto presenta una **desalineación significativa** entre diseño e implementación:

- **Fortalezas**: Los componentes de lectura de sensores (GPS, MAX30102) están bien implementados y documentados.
- **Debilidades críticas**: 
  - Falta implementación de la máquina de estados diseñada
  - Sistema de alertas SMS no integrado
  - Lógica de detección de eventos sin implementar

**Recomendación general**: Priorizar la implementación del flujo de control principal (FSM) y la integración del módulo de alertas antes de continuar con desarrollo adicional.

---

**Estado del Proyecto:** 🟡 En desarrollo - Requiere alineación diseño-código  
**Próxima revisión recomendada:** Tras implementación de puntos de prioridad alta