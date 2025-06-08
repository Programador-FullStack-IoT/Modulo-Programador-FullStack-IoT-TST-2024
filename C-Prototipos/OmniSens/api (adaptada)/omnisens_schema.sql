-- omnisens_optimizations.sql
--
-- Este script añade índices a la base de datos OmniSens para optimizar
-- el rendimiento de las consultas, especialmente en la tabla `measurements`.
-- Se recomienda ejecutarlo en la base de datos `AQI_DB` después de haber creado el esquema.

USE AQI_DB;

-- 1. Índice en la tabla `measurements` para búsquedas por dispositivo.
--    Acelera enormemente las uniones (JOINs) y las búsquedas con `WHERE d.device_eui = ?`.
CREATE INDEX idx_measurements_device_id ON measurements(device_id);

-- 2. Índice compuesto en `measurements` para la consulta más común:
--    buscar los datos de un dispositivo (`device_id`) y ordenarlos por fecha (`timestamp`).
--    Este es el índice más importante para la escalabilidad de las lecturas de datos históricos.
CREATE INDEX idx_measurements_device_timestamp ON measurements(device_id, timestamp DESC);

-- NOTA: Las columnas `devices.device_eui` y `sensor_types.name`
-- ya están indexadas de manera eficiente porque se definieron como `UNIQUE` en el esquema.
-- MySQL/MariaDB crea automáticamente un índice para las claves únicas y primarias.

SELECT 'Índices creados exitosamente en la tabla measurements.' AS status;