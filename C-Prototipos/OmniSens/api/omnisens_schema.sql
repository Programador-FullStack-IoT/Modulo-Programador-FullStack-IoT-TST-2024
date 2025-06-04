-- Proyecto OmniSens - Esquema MariaDB para AQI_DB
-- Ejecute este script en su instancia de MariaDB para la base de datos AQI_DB.

-- Tabla para almacenar información de los dispositivos
CREATE TABLE IF NOT EXISTS devices (
    device_id VARCHAR(255) PRIMARY KEY, -- Identificador único del dispositivo
    name VARCHAR(255),                  -- Nombre del dispositivo
    type VARCHAR(100),                  -- Tipo de dispositivo
    location VARCHAR(255),              -- Ubicación del dispositivo
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Fecha de creación
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP -- Fecha de última actualización
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Tabla para almacenar lecturas de datos de sensores
CREATE TABLE IF NOT EXISTS sensor_data (
    id INT AUTO_INCREMENT PRIMARY KEY,      -- Identificador único de la lectura
    device_id VARCHAR(255) NOT NULL,        -- Referencia al dispositivo
    timestamp DATETIME NOT NULL,            -- Fecha y hora de la lectura
    temperatura1 DECIMAL(5,2) NULL,         -- Temperatura 1
    temperatura2 DECIMAL(5,2) NULL,         -- Temperatura 2
    uv DECIMAL(5,2) NULL,                   -- Radiación ultravioleta
    humedad_relativa DECIMAL(5,2) NULL,     -- Humedad relativa
    presion_atm DECIMAL(7,2) NULL,          -- Presión atmosférica
    luz INT NULL,                           -- Nivel de luz
    gas_level VARCHAR(50) NULL,             -- Nivel de gas
    codigo_alarma VARCHAR(50) NULL,         -- Código de alarma
    received_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Fecha de recepción del dato
    INDEX idx_sensor_data_device_timestamp (device_id, timestamp), -- Índice para búsquedas rápidas por dispositivo y fecha
    FOREIGN KEY (device_id) REFERENCES devices(device_id) ON DELETE CASCADE -- Relación con la tabla devices, elimina datos si el dispositivo se elimina
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Tabla para almacenar actualizaciones de estado de actuadores
CREATE TABLE IF NOT EXISTS actuator_status (
    id INT AUTO_INCREMENT PRIMARY KEY,      -- Identificador único del estado
    device_id VARCHAR(255) NOT NULL,        -- Referencia al dispositivo
    timestamp DATETIME NOT NULL,            -- Fecha y hora del estado
    rele1_status BOOLEAN NULL,              -- Estado del relé 1
    rele2_status BOOLEAN NULL,              -- Estado del relé 2
    pwm_value INT NULL,                     -- Valor PWM
    received_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Fecha de recepción del estado
    INDEX idx_actuator_status_device_timestamp (device_id, timestamp), -- Índice para búsquedas rápidas por dispositivo y fecha
    FOREIGN KEY (device_id) REFERENCES devices(device_id) ON DELETE CASCADE -- Relación con la tabla devices, elimina datos si el dispositivo se elimina
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Tabla para registrar comandos enviados a los dispositivos
CREATE TABLE IF NOT EXISTS commands_log (
    id INT AUTO_INCREMENT PRIMARY KEY,      -- Identificador único del comando
    device_id VARCHAR(255) NOT NULL,        -- Referencia al dispositivo
    command TEXT NOT NULL,                  -- Comando enviado
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- Fecha de envío
    response TEXT NULL,                     -- Respuesta recibida
    response_received_at TIMESTAMP NULL,    -- Fecha de recepción de la respuesta
    FOREIGN KEY (device_id) REFERENCES devices(device_id) ON DELETE SET NULL -- Mantener el registro aunque el dispositivo sea eliminado
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Tabla para usuarios de la API (autenticación simple por clave API)
CREATE TABLE IF NOT EXISTS api_users (
    api_key VARCHAR(255) PRIMARY KEY,       -- Clave API única
    user_description VARCHAR(255),          -- Descripción del usuario
    is_active BOOLEAN DEFAULT TRUE,         -- Estado de la cuenta (activa/inactiva)
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- Fecha de creación
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

-- Insertar un usuario API por defecto (reemplace 'your-secret-api-key' por el que está en .env)
INSERT INTO api_users (api_key, user_description)
VALUES ('your-secret-api-key', 'Usuario API por defecto para OmniSens')
ON DUPLICATE KEY UPDATE user_description = VALUES(user_description), is_active = VALUES(is_active); -- Actualiza la descripción y el estado si ya existe