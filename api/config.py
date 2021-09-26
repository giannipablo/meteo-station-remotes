import os
from dotenv import load_dotenv

basedir = os.path.abspath(os.path.dirname(__file__))
load_dotenv(os.path.join(basedir, '.env'))


class Config():
    DB_URL = os.environ.get('DB_URL')
    DB_PORT = os.environ.get('DB_PORT')
    DB_USER  = os.environ.get('DB_USER')
    DB_PASS  = os.environ.get('DB_PASS')
    DB_DATABASE = os.environ.get("DB_DATABASE")
    DB_COLLECTION = os.environ.get("DB_COLLECTION")