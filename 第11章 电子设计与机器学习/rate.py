import numpy as np

''' 生成 测试集与训练集  数据  '''
X = 2 * np.random.rand(100, 1)
y = 4 + 3 * X + np.random.randn(100, 1)  # 初始化参数为 4 与 3
# plt.figure()      # 创建图层
# plt.scatter(X, y) # 画出散点

''' 初始化参数  '''
n_epochs = 50    # epoch = 50次
t0, t1 = 5, 50     # 学习速率表的超参数
X_b = np.c_[np.ones((100, 1)), X]    # 偏置
eta = 0.5           # 学习率η
m = 100           # 训练样本总数

def learning_schedule(t):
    return t0 / (t + t1)

theta = np.random.randn(2,1)  # 随机初始化参数θ

'''  梯度下降  '''
for epoch in range(n_epochs):
    if epoch < 10:
        tx = np.linspace(0, 2, 50)
        ty  = tx * theta[0] + theta[1]
        # if epoch != 0:
        #     plt.plot(tx, ty,'r-', linewidth=1.0)
        # else:
        #     plt.plot(tx, ty, 'r--', linewidth=1.0)
    for i in range(m):
        random_index = np.random.randint(m)    # 随机抽取样本
        xi = X_b[random_index:random_index+1]  # 随机抽取样本 x
        yi = y[random_index:random_index+1]    # 随机抽取样本 y
        gradients = 2 * xi.T.dot(xi.dot(theta) - yi)  #  m =1 
        eta = learning_schedule(epoch * m + i)
        theta = theta - eta * gradients
    
'''  打印输出 '''
# plt.show()
print(theta)
