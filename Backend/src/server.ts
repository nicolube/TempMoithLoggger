import express from 'express'
import { RequestHandler } from './request_handler';
import config from "./config.json"
import { DatabaseManager } from './database_manager';
const app = express();

const db = new DatabaseManager();
new RequestHandler(app, db);

app.listen(config.port, config.host, () => console.log('Listening on Port http://'+config.host+':'+config.port));