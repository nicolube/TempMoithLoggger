

export interface ApiKeyDocument {
    id?: number;
    key: string;
    created_at: string;
}

export interface LogDataDocument {
    uuid: string;
    name: string;
    temp: number;
    humidity: number;
    timediff?: number;
}