
import tensorflow as tf
import matplotlib
import matplotlib.pyplot as plt

'''  加载数据集 '''
mnist = tf.keras.datasets.mnist
(x_train, y_train),(x_test, y_test) = mnist.load_data()
# 将（-1,28,28）的图片变成(-1,28,28,1) 
x_train = x_train.reshape((-1,28,28,1))
x_test = x_test.reshape((-1,28,28,1))
# 归一化处理
x_train, x_test = x_train / 255.0, x_test / 255.0


''' --> 构建CNN模型 '''
model = tf.keras.models.Sequential([
tf.keras.layers.Conv2D(input_shape=(x_train.shape[1], 
                     x_train.shape[2], x_train.shape[3]),
                          filters=32,kernel_size=(3,3), strides=(1,1),
                          padding='valid',activation='relu'),
    tf.keras.layers.MaxPool2D(pool_size=(2,2)),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Dropout(0.2),
    tf.keras.layers.Dense(10, activation='softmax')
])

'''--> 定义损失函数与优化器  '''
model.compile(optimizer=tf.keras.optimizers.Adam (), # 定义优化器
            loss=tf.keras.losses.sparse_categorical_crossentropy, # 定义损失函数
            metrics=['accuracy'])               # 计算预测正确率



'''--> 训练模型  '''
history = model.fit(x_train, y_train, epochs=4,validation_split=0.2)

