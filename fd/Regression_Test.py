import hashlib
import xml.etree.ElementTree as ET
from optparse import OptionParser

statistics = {"Tests":0,"FailuresTotal":0,"Errors":0,"Failures":0}

def md5(file_name):
    hash_md5 = hashlib.md5()
    with open(file_name, "rb") as read_file:
        for chunk in iter(lambda: read_file.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

def parseSuccessfulTests(file_name):
    stats_dict = statistics.copy()
    tree = ET.parse(file_name)
    root = tree.getroot()
    for child in root:
        if child.tag == "Statistics":
    	    for sub_result in child:
    	        for sub_key in statistics.keys():
    	            if sub_key == sub_result.tag:
    	            	stats_dict[sub_key] = int(sub_result.text)
    return stats_dict

parser = OptionParser()
parser.add_option('-f', '--file', dest="file_passed",
                    help="the file to be passed and read as an option")
parser.add_option('-c', '--compare', dest="compare_file",
                    help="the compared filed to be passed and read as a comparison")

(options, args) = parser.parse_args()


original_file = md5(options.file_passed)
compare_file  = md5(options.compare_file)
original_stats = parseSuccessfulTests(options.file_passed)
compare_stats  = parseSuccessfulTests(options.compare_file)

if original_stats != compare_stats:
    print "Compared file:" + str(options.file_passed) + " and " + str(options.compare_file) + ". They are different, here are results:"
    print "File:" + str(options.file_passed) + " stats:" + str(original_stats)
    print "File:" + str(options.compare_file) + " stats:" + str(compare_stats)
else:
    print "Compared file:" + str(options.file_passed) + " and " + str(options.compare_file) + ". They are the same results"
