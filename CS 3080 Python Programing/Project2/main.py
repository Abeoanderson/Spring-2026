#main.py
import MyFirstPackage as mfp
from MyFirstPackage import add
Result1= mfp.add(2,3)
Result2 = mfp.sub(5,2)
Result3 = add(2,3) # this shows we are using add that was imported individually from MyFirstPackage
print(f"result from MyFirstPackage.add is: {Result1}")
print(f"result from MyFirstPackage.sub is: {Result2}")
print(f"result from add imported individually: {Result3}")