

import tensorflow as tf

mnist = tf.keras.datasets.mnist   
(x_train, y_train),(x_test, y_test) = mnist.load_data()    # 导入mnist 数据集

# 归一化处理
(x_train, y_train),(x_test, y_test) = mnist.load_data()
x_train, x_test = x_train / 255.0, x_test / 255.0

''' 定义神经网络 输入层1个神经元，隐含层10个神经元，输出层1个神经元  '''

model = tf.keras.models.Sequential([
  tf.keras.layers.Flatten(input_shape=(28, 28)),    # 定义输入层
  tf.keras.layers.Dense(128, activation='relu'),    # 定义隐含层
  tf.keras.layers.Dense(10, activation='softmax')   # 定义输出层
])

'''  定义损失函数与优化器  '''
model.compile(optimizer=tf.keras.optimizers.SGD (0.1), # 定义优化器
            loss=tf.keras.losses.sparse_categorical_crossentropy,  # 定义损失函数
            metrics=['accuracy'])                             # 计算预测正确率


'''  模型训练与保存  '''
model.fit(x_train, y_train, epochs=3)           # 训练模型
model.save('my_mnist_model.h5')                 # 保存模型

