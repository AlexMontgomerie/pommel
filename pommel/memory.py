import xmltodict

class memory:

    def __init__(self, memory_path):

        # load memory
        with open(memory_path, "r") as f:
            self.memory = xmltodict.parse(f.read())

        # add fields
        self.name = self.get_parameter_value(self.memory["memspec"]["parameter"], "memoryId")

        # add fields
        self.data_width = int(self.get_parameter_value(self.memory["memspec"]["memarchitecturespec"]["parameter"], "width"))
        self.chips = int(self.get_parameter_value(self.memory["memspec"]["memarchitecturespec"]["parameter"], "nbrOfChips"))
        self.banks = int(self.get_parameter_value(self.memory["memspec"]["memarchitecturespec"]["parameter"], "nbrOfBanks"))
        self.cols = int(self.get_parameter_value(self.memory["memspec"]["memarchitecturespec"]["parameter"], "nbrOfColumns"))
        self.rows = int(self.get_parameter_value(self.memory["memspec"]["memarchitecturespec"]["parameter"], "nbrOfRows"))
        self.data_rate = int(self.get_parameter_value(self.memory["memspec"]["memarchitecturespec"]["parameter"], "dataRate"))
        self.clock = int(self.get_parameter_value(self.memory["memspec"]["memtimingspec"]["parameter"], "clkMhz"))

        # other parameters
        self.num_dq = self.chips*self.data_width
        self.bandwidth = self.data_rate*self.clock*self.num_dq/(8*1000.0)
        self.capacity = self.banks*self.cols*self.rows*self.data_width
        self.total_capacity = self.chips*self.banks*self.cols*self.rows*self.data_width

    def get_parameter_value(self, parameters, field):
        # iterate over parameters in xml doc
        for parameter in parameters:
            if parameter["@id"] == field:
                return parameter["@value"]

