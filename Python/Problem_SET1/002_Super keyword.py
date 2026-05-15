class Animal:
    species =""
    def __init__(self,name,breed):
        self.name=name
        self.breed=breed
    def sound (self):
        print("makes a sound")
class Bird(Animal):
    species="Bird"
    def __init__(self, name,breed,wingspan):
        super().__init__(name,breed)
        self.wingspan=wingspan
    def sound(self):
        print(f"{self.name} says Chirping")    

b1= Bird("Humming Bard","small bird",10)
print(b1.name)
b1.sound()