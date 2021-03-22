import xmltodict

class memory:

    def __init__(self, memory_path):

        # load memory
        with open(memory_path, "r") as f:
            self.memory = xmltodict.parse(f.read())
        
        # add fields
        self.chips = int(self.memory["memspec"]["memarchitecturespec"]["parameter"][1]["@value"])
