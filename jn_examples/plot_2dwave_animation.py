#!/usr/bin/env python
# coding: utf-8

# In[1]:


# This reads a csv file using pandas dataframe
# and copy data to numpy arrays to make a plot.
# animation from
# https://matplotlib.org/2.0.2/examples/animation/simple_anim.html
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import matplotlib.animation as animation

# Enable interactive plot
# get_ipython().run_line_magic('matplotlib', 'notebook')

df = pd.read_csv('wave2Dfull.csv')
print(df)

t=(df["t"].to_numpy())
x=(df["x"].to_numpy())
y=(df["y"].to_numpy())
z=(df["z"].to_numpy())


#  fetch data for time==0
tfilt= t==0      # filter for t==0 
xval=x[tfilt]
yval=y[tfilt]
zval=z[tfilt]

xval2d=xval.reshape((51,51))
yval2d=yval.reshape((51,51))
zval2d=zval.reshape((51,51))

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

#line, =ax.plot_wireframe(xval2d,yval2d,zval2d)
line=None

def animate(i):
    global line
    global xval2d,yval2d,z
    print("i=",i)
    it=i%51
    tfilt= t==it
    zval=z[tfilt]
    zval2d=zval.reshape((51,51))
    print("zval2D",zval2d)
    if line:
        ax.collections.remove(line)
    # if line:
    #      ax.clf()
    line=ax.plot_wireframe(xval2d,yval2d,zval2d)
    return line

def init():
    global line
    print("ini")
    # if line:
    #      ax.clf()    
    if line:
        ax.collections.remove(line)
    line=ax.plot_wireframe(xval2d,yval2d,zval2d)
    return line

ax.set_zlim3d([-1.0,1.0])
print("ani")
ani=animation.FuncAnimation(fig,animate,np.arange(0,10000),init_func=init,interval=100,blit=False)

print("after ani")  
# plt.savefig('plot_2dwave_t1.png')
plt.show()


# In[ ]:





# In[ ]:




