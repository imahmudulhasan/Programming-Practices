class Animal:
    species ="Mammal"
    def __init__(self,name,breed):
        self.name=name
        self.breed=breed
    def sound (self):
        print("makes a sound")

class Dog (Animal):

    def sound(self):
        print(f"{self.name} says woof!")

class Cat (Animal):
    def sound(self):
        print(f"{self.name} says Meow")   

# d = Dog("bruno","grey")
# c= Cat("Fluffy","white")
# print(d.name)
# d.sound()
# print(c.name)
# c.sound()
# print(f"the species of {c.name} is {c.species}")



