import seaborn
import pandas
import sqlite3
import datetime
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker


def get_data():
    connection = sqlite3.connect('measures.db')
    sql = "select strftime('%d %H:%M',time) as time, PM01_0, PM02_5, PM10_0 from measures where time > '{}'"
    #sql = "select time(time) as time, PM01_0, PM02_5, PM10_0 from measures where time > '{}'"
    delta = datetime.timedelta(hours=24)
    query = sql.format(datetime.datetime.now() - delta)
    return pandas.read_sql_query(query, connection)


def set_tick_properties(data_size):
    fig, ax = plt.subplots()
    if data_size > 10:
    	mp = ticker.MultipleLocator(data_size/10)
    	ax.xaxis.set_major_locator(mp)

data = get_data()

seaborn.set()
seaborn.set_style("darkgrid", {"axes.facecolor": ".9", 'grid.linestyle': '--'})

set_tick_properties(len(data))

data = pandas.melt(data, ['time'])

plt.ylim(0, max(200, min(data['value'].max(), 300)))

plot = seaborn.lineplot(
    palette="deep",
    x='time',
    y='value',
    hue='variable',
    data=data
)

plot.tick_params('x', labelrotation=21)

plot.figure.savefig('current.png')
