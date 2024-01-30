# distutils: language = c++

from libcpp.string cimport string
from libcpp.unordered_set cimport unordered_set
import json

cdef extern from "vns-cpp/naming.cpp":
    pass

cdef extern from "vns-cpp/naming.h":
    cdef cppclass Naming:
        Naming(string)
        string to_string() const
        string get_name() const
        unordered_set[string] get_tags() const
        bint contains_tag(string) const
        void insert_tag(string)
        void erase_tag(string)
        bint equal(string, bint) const
        @staticmethod
        string get_database_as_json()
        @staticmethod
        void update_database(string&)

# Wrapper class for Naming
cdef class PyNaming:
    cdef Naming *thisptr 

    def __str__(self) -> str:
        return self.to_string()

    def __cinit__(self, str name):
        self.thisptr = new Naming(name.encode('utf-8', errors='strict'))

    def __dealloc__(self):
        del self.thisptr

    def to_string(self):
        return self.thisptr.to_string().decode('utf-8')

    def get_name(self):
        return self.thisptr.get_name().decode('utf-8')

    def get_tags(self):
        return {t.decode('utf-8') for t in self.thisptr.get_tags()}

    @property
    def name(self):
        return self.get_name()

    @property
    def tags(self):
        return self.get_tags()

    def contains_tag(self, str tag):
        return self.thisptr.contains_tag(tag.encode('utf-8', errors='strict'))

    def insert_tag(self, str tag):
        self.thisptr.insert_tag(tag.encode('utf-8', errors='strict'))

    def erase_tag(self, str tag):
        self.thisptr.erase_tag(tag.encode('utf-8', errors='strict'))

    def equal(self, tag, bint must_be_the_same = False):
        return self.thisptr.equal((tag if isinstance(tag, str) else tag.to_string()).encode('utf-8', errors='strict'), must_be_the_same)

    @staticmethod
    def get_database():
        return json.loads(Naming.get_database_as_json().decode('utf-8'))

    @staticmethod
    def update_database(dict data):
        Naming.update_database(json.dumps(data).encode('utf-8', errors='strict'))
