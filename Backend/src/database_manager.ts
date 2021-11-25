
import config from "./config.json"
import mariadb, { Connection } from "mariadb"
import { exit } from "process";
import fs from "fs";
import { ApiKeyDocument, LogDataDocument } from "./documents";

export class DatabaseManager {
    con?: Connection

    constructor() {
        mariadb.createConnection({
            user: config.database.user, 
            password: config.database.passowrd,
            port: config.database.port,
            host: config.database.host,
            database: config.database.database
        }).then(con => {
            this.con = con;
            console.log("Database connected...")
            this.init();
        }).catch(err => {
            console.log("Failed to connect to database: "+err)
            exit();
        });
    }

    private init(): void {
        console.log("Create default data... ");
        var cmdList = fs.readFileSync("src/default.sql").toString().split(';')
        cmdList.pop();
        cmdList.forEach(cmd => {
            this.con?.query(cmd+';');
        });
    }

    putSensorData(sd: LogDataDocument) {
        this.con?.query("INSERT INTO sensor_log (uuid, name, type, value) VALUES (?, ?, 'humidity', ?);", [sd.uuid, sd.name, sd.humidity]);
        this.con?.query("INSERT INTO sensor_log (uuid, name, type, value) VALUES (?, ?, 'temparatue', ?);", [sd.uuid, sd.name, sd.temp]);
        this.con?.query("UPDATE sensor_log SET name=? WHERE uuid=? AND name != ?", [sd.name, sd.uuid, sd.name]);
    }

    async getToken(token: string) : Promise<ApiKeyDocument|null> {
        const result =  await this.con?.query('SELECT k.id as keyId, k.`key`, k.created_at as keyCreated_at, k.user_id\
        FROM api_keys k WHERE k.`key`=?;', [token]);
        if (result.length <= 0) {
            return null;
        }
        return this.makeApiKeyDocument(result[0])
    }

   

    makeApiKeyDocument(res: any) : ApiKeyDocument {
        return {
            id: res.keyId as number,
            key: res.key as string,
            created_at: res.keyCreated_at as string
        }
    }


}