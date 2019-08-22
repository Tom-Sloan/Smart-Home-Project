# Importing the libraries
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

# Get file names 

path = os.getcwd()

folders = []

# r=root, d=directories, f = files
for r, d, f in os.walk(path):
    for folder in d:
        print(folder)
        folders.append(os.path.join(r, folder))

for f in folders:
    print(f)

# Load in all files

ds_move = []
ds_var = [];
ds_std = [];
ds_slp = [];
ds_mean = []

for x in range(len(folders)):
   temp = os.listdir(folders[x])
   os.chdir(folders[x])
   #print("Temp is changing folders to ", temp)
   for c in range(len(os.listdir(folders[x]))):
       print(x);
       dataset = pd.read_csv(temp[c], sep=",", header=None);
       dataset.columns = ["acc_x", "acc_y", "acc_z", "gyr_x", "gyr_y", "gyr_z", "mag_x", "mag_y", "mag_z"];
       ds_move.append(x);
       ds_mean.append(dataset.mean().tolist());
       ds_var.append(dataset.var().tolist());
       ds_std.append(dataset.std().tolist());
       ds_slp.append(dataset.apply(lambda x: np.polyfit(dataset.index, x, 1)[0]).tolist());
       
       

# separate the 2
# =============================================================================
#    print("---------------------------------------")  
#    means.append(ds_mean)
#    var.append(ds_var)
#    std.append(ds_std)
#    slp.append(ds_slp)
#    ds_var = []
#    ds_slp = []
#    ds_std = []
#    ds_mean = [] 
# =============================================================================

#visulizing slopes
# =============================================================================
# 
# for i in dataset.columns:
#     plt.scatter(dataset.index, dataset[i], label=i)
#     plt.plot(np.polyval(np.polyfit(dataset.index, dataset[i], 1), dataset.index))
# 
# plt.legend()
# plt.show()
# 
# 
# # Visualize
# plt.figure(12)
# plt.plot(dataset.acc_x)
# 
# plt.plot(dataset.acc_y)
# 
# plt.plot(dataset.acc_z)
# 1629








# =============================================================================


test_1 = [-2401, 15039, 813, -303, -5589, 599, 870, -3964]
test_2 = ds_mean[14]
test_3 = ds_mean[600]
tests = [test_1, test_2, test_3]

# Splitting the dataset into the Training set and Test set
from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test = train_test_split(ds_mean, ds_move, test_size = 0.25)

# Feature Scaling
from sklearn.preprocessing import StandardScaler
sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)

# Fitting SVM to the Training set
from sklearn.svm import SVC
classifier = SVC()
classifier.fit(X_train, y_train)

# Predicting the Test set results
y_pred = classifier.predict(X_test)
y_ex = classifier.predict(test_1)
# Making the Confusion Matrix
from sklearn.metrics import confusion_matrix, accuracy_score
cm = confusion_matrix(y_test, y_pred)
acc = accuracy_score(y_test, y_pred)

import coremltools
input_features = ["acc_x_mean", "acc_y_mean", "acc_z_mean", "gyr_x_mean", "gyr_y_mean", "gyr_z_mean", "mag_x_mean", "mag_y_mean", "mag_z_mean"]
output_feature = "Movement"

model = coremltools.converters.sklearn.convert(classifier, input_features, output_feature)
model.save("movement.mlmodel")
