import json
import logging
import pymongo 

from config import Config
from datetime import datetime
from typing import Optional
from fastapi import FastAPI#, status
from pydantic import BaseModel
from uuid import UUID


#####################################
# Setup loggers
with open('./logging_config.json', 'r') as f:
    dictConf = json.loads(f.read())
logging.config.dictConfig(dictConf)

# get root logger
logger = logging.getLogger(__name__)


#####################################
# App
app = FastAPI()


#####################################
# Models 
class Payload(BaseModel):
    """
    This model aims to capture a minimum set of measurements.
    In the future, if the sensors population is diverse, it would be 
    better to implement a list of objects with a measurement description.
    """
    temperature : Optional[float] = None
    wind_speed : Optional[float] = None
    wind_direction : Optional[str] = None
    preasure : Optional[float] = None

class RemoteData(BaseModel):
    uuid : UUID
    dev_name : str
    time_stamp : datetime 
    payload : Optional[Payload] = None



#####################################
# Endpoit to push/insert elements to the DB
@app.post("/push")
async def push(remote_data:RemoteData):
    # Guet the logger
    logger = logging.getLogger('main.push')

    try:
        # Conect to the database
        uri = f'mongodb://{Config.DB_USER}:{Config.DB_PASS}@{Config.DB_URL}:{Config.DB_PORT}/'

        client = pymongo.MongoClient(uri)
        
        push = client[Config.DB_DATABASE][Config.DB_COLLECTION].insert_one(remote_data.dict())
        pushed = client[Config.DB_DATABASE][Config.DB_COLLECTION].find_one({"_id":push.inserted_id})
        
        logger.info('Data insertion completed')
        return {"status" : "OK"}

    except Exception as e:
        logger.error(f'Exception {e} with data: {remote_data}') 
        return {"status" : "FAIL"}


