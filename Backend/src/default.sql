CREATE TABLE IF NOT EXISTS api_keys (
	id BIGINT auto_increment KEY NOT NULL,
	`key` varchar(64) NOT NULL UNIQUE,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL
)
ENGINE=InnoDB
DEFAULT CHARSET=utf8mb4
COLLATE=utf8mb4_general_ci;

CREATE TABLE IF NOT EXISTS sensor_log (
	id BIGINT auto_increment KEY NOT NULL,
	uuid varchar(40) NOT NULL,
	`name` varchar(32) NOT NULL,
	`type` varchar(32) NOT NULL,
	`value` FLOAT NOT NULL, 
	`time` DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL,
	created_at DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL
)
ENGINE=InnoDB
DEFAULT CHARSET=utf8mb4
COLLATE=utf8mb4_general_ci;

