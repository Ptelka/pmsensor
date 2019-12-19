import seaborn
import pandas
import sqlite3
import datetime
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker


def normalize(value, min_val, max_val):
    return (value - min_val) / (max_val - min_val)


def get_data():
    connection = sqlite3.connect('measures.db')
    #sql = "select strftime('%H:%M',time) as time, PM01_0, PM02_5, PM10_0 from measures where time > '{}'"
    sql = "select time(time) as time, PM01_0, PM02_5, PM10_0 from measures where time > '{}'"
    delta = datetime.timedelta(minutes=30)
    query = sql.format(datetime.datetime.now() - delta)
    return pandas.read_sql_query(query, connection)


def set_tick_properties(data_size):
    res = normalize(data_size, 1, 10)
    fig, ax = plt.subplots()
    mp = ticker.MultipleLocator(res)
    ax.xaxis.set_major_locator(mp)


data = get_data()
print(len(data))
seaborn.set()
seaborn.set_style("darkgrid", {"axes.facecolor": ".9", 'grid.linestyle': '--'})

set_tick_properties(len(data))

plt.ylim(0, 250)

plot = seaborn.lineplot(
    palette="deep",
    x='time',
    y='value',
    hue='variable',
    data=pandas.melt(data, ['time'])
)
plot.tick_params('x', labelrotation=30)

plot.figure.savefig('current.png')
