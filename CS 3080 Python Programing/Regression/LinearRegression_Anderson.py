from FileUtils_Anderson import FileUtils as futils
import numpy as np
import matplotlib.pyplot as plt
import random

# open data
data_file = "rounded_hours_student_scores.csv"
data = futils.read_csv_file(data_file, "dictionary")
random.shuffle(data)


X = []
Y = []
for row in data:
    X.append(float(row['Hours']))
    Y.append(float(row['Scores']))  

# make x and x_test as np arrays of X
x = np.array(X[:50], dtype=float)
x_test = np.array(X[50:], dtype=float)

# make y and y_test as np arrays of Y
y = np.array(Y[:50], dtype=float)
y_test = np.array(Y[50:], dtype=float)

#create x_mean and y_mean
x_mean = np.mean(x)
y_mean = np.mean(y)

nominator = np.sum((x - x_mean) * (y - y_mean))
denominator = np.sum((x - x_mean) ** 2)
m = nominator / denominator
c = y_mean - m * x_mean

def predict(x, m, c):
    return m * x + c


prediction = predict(x, m, c)
test_predictions = predict(x_test, m, c)

def mean_squared_error(y_gt, y_pred):
    return np.mean((y_gt - y_pred)**2)

train_mse = mean_squared_error(y, prediction)
test_mse = mean_squared_error(y_test, test_predictions)
print(f"Train Mean Squared Error (y - predictions): {train_mse}")
print(f"Test Mean Squared Error (y_test - test_predictions): {test_mse}")

train_results = [{'Hours': x[i], 'Score': y[i], 'Predicted Score': prediction[i]} for i in range(len(x))]
print(train_results)
# Save train results to a CSV file
for row in train_results:
    futils.write_csv_file("train_results.csv", row)

test_results = [{'Hours': x_test[i], 'Score': y_test[i], 'Predicted Score': test_predictions[i]} for i in range(len(x_test))]
# Save test results to a CSV file
for row in test_results:
    futils.write_csv_file("test_predictions.csv", row)  

# Plotting the results
plt.scatter(X, Y, color='blue', label='Actual data')
plt.plot(x, prediction, color='red', label='Regression line')
plt.plot(x_test, test_predictions, color='black', label='Test data')
plt.xlabel('Hours')
plt.ylabel('Scores')
plt.legend()
plt.title('Linear Regression From Scratch')
plt.show()