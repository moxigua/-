
import face_recognition   # 包含库

# 加载图片
picture_of_me = face_recognition.load_image_file("me.jpg")  
# 提取人脸特征,在my_face_encoding 变量中包含了【me.jpg】图片中人脸的特征，通过此人脸特征可与任何人脸进行匹配。
my_face_encoding = face_recognition.face_encodings(picture_of_me)[0]  

# 加载未知人脸的图片
unknown_picture = face_recognition.load_image_file("unknown.jpg")  
unknown_face_encoding = face_recognition.face_encodings(unknown_picture)[0] 

# 通过compare_faces()函数比较my_face_encoding 与 unknown_picture 后，可判定这两个图片是否为同一个人
results = face_recognition.compare_faces([my_face_encoding],
                                                   unknown_face_encoding)

if results[0] == True:
    print("It's a picture of me!")
else:
	print("It's not a picture of me!")

