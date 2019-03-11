from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.triggers.interval import IntervalTrigger


class Scheduler(object):
    def __init__(self, job, interval):
        self.bs = BackgroundScheduler()
        self.bs.start()
        self.bs.add_job(
            func=job,
            trigger=IntervalTrigger(seconds=interval),
            id='prices_retrieval_job',
            name='Retrieve cryptocurrency prices every 10 seconds',
            replace_existing=True
        )
