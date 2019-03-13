import os
basedir = os.path.abspath(os.path.dirname(__file__))


class Config(object):
    DEBUG = False
    TESTING = False
    CSRF_ENABLED = True
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'dummy_secret'
    SQLALCHEMY_COMMIT_ON_TEARDOWN = True


class StagingConfig(Config):
    DEVELOPMENT = True
    DEBUG = False


class DevelopmentConfig(Config):
    DEVELOPMENT = True
    DEBUG = False


class TestingConfig(Config):
    TESTING = True


class ProductionConfig(Config):
    DEBUG = False