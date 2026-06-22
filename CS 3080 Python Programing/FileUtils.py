import os
import json
import csv
from unittest import case
class FileUtils:
    @staticmethod
    def path_exists(filename):
        return os.path.exists(filename)
    
    @staticmethod
    def write_text_file(filename, content):
        mode = 'a' if FileUtils.path_exists(filename) else 'w'
        with open(filename, mode, encoding='utf-8') as f:
            match content: 
                case list() | tuple() if all(isinstance(item, str) for item in content):
                    f.writelines(content)           
                case list() | tuple() if not all(isinstance(item, str) for item in content):
                    f.writelines([str(item) + '\n' for item in content])     
                case str():
                    f.write(content + '\n' )
                case dict():
                    for key, value in content.items():
                        f.write(f"{key}: {value}\n")
                case _:
                     f.write(str(content)+"\n")
    @staticmethod
    def read_text_file(filename, to="list_string"): 
        if not FileUtils.path_exists(filename):
            raise FileNotFoundError(f"The file '{filename}' does not exist.")
        with open(filename, 'r', encoding='utf-8') as f:
            if to == "list_string":
                return f.read().split()
            elif to == "list_int":
                return [int(x) for x in f.read().split()]
            elif to == "list_float":
                return [float(x) for x in f.read().split()]
            elif to == "raw_dictionary":
                return {i: line.strip() for i, line in enumerate(f.readlines())}
            elif to == "formatted_dictionary":
                return {line.split(":")[0].strip(): line.split(":")[1].strip() for line in f.readlines()}
            
    @staticmethod
    def write_csv_file(filename, data):
        mode = 'a' if FileUtils.path_exists(filename) else 'w'
        with open(filename, mode, newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            if isinstance(data, dict):
                fieldnames = data.keys()
                writer = csv.DictWriter(f, fieldnames=fieldnames)
                if mode == "w":
                    writer.writeheader()
                writer.writerow(data)
            elif isinstance(data, (list,tuple)):
                writer = csv.writer(f)
                writer.writerow(data)
    @staticmethod
    def read_csv_file(filename, to="list"):
        with open(filename, 'r', encoding='utf-8') as f:
            if to == "dictionary": 
                reader = csv.DictReader(f)
                return [row for row in reader]
            reader = csv.reader(f)
            if to == "list_string":
                return [row for row in reader]
            elif to == "list_int":
                return [[int(item) for item in row] for row in reader]
            elif to == "list_float":
                return [[float(item) for item in row] for row in reader]
            elif to == "raw_dictionary":
                return [{i: item for i, item in enumerate(row)} for row in reader]
    @staticmethod
    def write_json_file(filename, data):
        if FileUtils.path_exists(filename):
            with open(filename, 'r', encoding='utf-8') as f:
                existing = json.load(f)
                if isinstance(existing, list):
                    existing.append(data)
                else:
                    existing = [existing, data]
        else:
            existing = data
            with open(filename, 'w', encoding='utf-8') as f:
                json.dump(existing, f, indent=4)
    @staticmethod
    def read_json_file(filename):
        with open(filename, 'r', encoding='utf-8') as f:
            return json.load(f)


class DemoUtils:
    filename = "demo"
    demo_number = 0
    filename = f"{filename}_{demo_number}"
