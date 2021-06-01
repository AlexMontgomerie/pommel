import xmltodict

class accelerator:

    def __init__(self, accelerator_path):
        # load accelerator 
        with open(accelerator_path, "r") as f:
            self.accelerator = xmltodict.parse(f.read())
        
        # add fields
        self.name = self.accelerator["acceleratorspec"]["@name"]
