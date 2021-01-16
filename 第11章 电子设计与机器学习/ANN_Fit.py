
import tensorflow as tf
from tensorflow.keras import layers
import numpy as np
#import matplotlib.pyplot as plt

'''  生成 测试集与训练集  数据  '''
# 生成-0.8A到0.8A间均匀发布的电流值（500个点），
# 同时将数据变为二维数据，一行代表一个输入矩阵
I_train = np.linspace(-0.8, 0.8, 500)[:, np.newaxis]
I_test = np.linspace(-0.8, 0.8, 500)[:, np.newaxis]
# 生成一些噪音数据，模拟采样噪声，如何正态分布
noise_train = np.random.normal(0, 0.05, I_train.shape)
# 根据欧姆定律生成的期望电压值
V_train = 0.8*I_train + noise_train
V_test = 0.8*(I_test) 


''' 定义神经网络 输入层1个神经元，隐含层10个神经元，输出层1个神经元  '''

model = tf.keras.Sequential()     # 生成网络模型的序列实例

# 增加10个神经元的隐含层，输入为1个神经元，使用input_shape指定输入个数
model.add(layers.Dense(10,input_shape=(1,) , activation=tf.nn.relu)) 
# Add a softmax layer with 10 output units:
model.add(layers.Dense(1, activation=None))


'''  定义损失函数与优化器  '''
model.compile(optimizer=tf.keras.optimizers.SGD (0.1), # 定义优化器
              loss=tf.losses.mean_squared_error)    # 定义损失函数


'''  训练人工神经网络  '''
for i in  range (501):
    model.fit(I_train, V_train,batch_size=500 )
    if(i==2) or (i ==5) or (i==100) or (i==200) or (i ==300) or (i==500):
        predict = model.predict(I_test)
        # plt.figure()  # 创建图层
        # plt.title("epoch = {}".format(i))
        # plt.scatter(I_train, V_train)  # 画出散点
        # plt.plot(I_test, predict, 'r-', lw=5)  # 画出实际预测曲线
        # plt.show()  # 显示曲线

