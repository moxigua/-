
import tensorflow as tf
import matplotlib.pyplot as plt

'''  加载数据集 '''
mnist = tf.keras.datasets.mnist
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train, x_test = x_train / 255.0, x_test / 255.0

'''  构建RNN模型 '''
simple_RNN_layer = tf.keras.layers.SimpleRNN(
					units = 64,              # 64 神经元
					input_shape=(28, 28))

model = tf.keras.models.Sequential([
    simple_RNN_layer,   # 循环神经网络层
    tf.keras.layers.BatchNormalization(),  # 批量正则化
    tf.keras.layers.Dense(10, activation='softmax')]   # 全连接层
)

'''  定义损失函数与优化器  '''
model.compile(loss='sparse_categorical_crossentropy',
              optimizer=tf.keras.optimizers.Adam(),
              metrics=['accuracy'])

'''  训练模型  '''
history = model.fit(x_train, y_train,
          validation_data=(x_test, y_test),
          batch_size=64,                  # 最小批量为64
          epochs=5)
