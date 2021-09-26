# import 
from datetime import datetime
from typing import Optional
from fastapi import FastAPI
from pydantic import BaseModel
from uuid import UUID

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
async def root(remote_data:RemoteData):
    print(remote_data)

    return {"message": "Hello World"}


