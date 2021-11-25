import cookieParser from 'cookie-parser';
import express, { Express, Request, Response } from 'express';
import bearerToken from 'express-bearer-token';
import cors from 'cors'
import { DatabaseManager } from './database_manager';
import { LogDataDocument } from './documents';

export class RequestHandler {
    app: Express;
    db: DatabaseManager;

    constructor(app: Express, db: DatabaseManager) {
        this.app = app;
        this.db = db;

        app.use(bearerToken())
        .use(cookieParser())
        .use(express.json())
        .use(cors())

        app.use(async (req, res, next) => {
            if (req.token != undefined) {
                var token = await db.getToken(req.token as string);
                if (token != null) {
                    next();
                    return;
                }
            }
            res.status(401).send();
        })

        app.post("/mcu/logdata", async (req, res) => {
            try {
                let doc = req.body as LogDataDocument;
                console.log(doc);
                db.putSensorData(doc);
            } catch(e) {
                console.log("Body: "+req.body);
                console.log("Error:"+e);
                res.status(400).send(e);
            }
        })
    }

    




}