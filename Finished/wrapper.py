import re
import os
import shutil
import random
import xml.etree.ElementTree as ET

def parse_csd_file(file_path):
    properties = {}
    with open(file_path, 'r') as file:
        for line in file:
            channel_matches = re.findall(r'channel\("([^"]+)"\)', line)
            range_matches = re.findall(r'range\(([^)]+)\)', line)
            for channel_match, range_match in zip(channel_matches, range_matches):
                range_values = [float(val) for val in range_match.split(',')]
                properties[channel_match] = range_values
    return properties

def generate_xml(properties, csd_file):
    
    file_name_without_extension = os.path.splitext(os.path.basename(csd_file))[0]
    
    xml_header = '<?xml version="1.0" encoding="utf-8"?>\n\n<PluginModule>\n\t<SourcePlugin Name="'+ file_name_without_extension +'"  CompanyID="64" PluginID="0">\n\t\t<PluginInfo>\n\t\t\t<PlatformSupport>\n\t\t\t\t<Platform Name="Any"></Platform>\n\t\t\t</PlatformSupport>\n\t\t</PluginInfo>\n\t'
    xml_footer = '\n\t</SourcePlugin>\n</PluginModule>'
    
    root = ET.Element("Properties")
    audio_engine_prop_id_counter = 0
    for channel, values in properties.items():
        root.text = "\n\n\t"
        prop = ET.SubElement(root, "Property", Name=channel, Type="Real32", SupportRTPCType="Exclusive", DisplayName=channel)
        
        prop.text = "\n\t\t"
        
        ui = ET.SubElement(prop, "UserInterface", Step="0.001", Fine="4", Decimals="3")
        ui.tail = "\n\t\t"
        
        default_value = ET.SubElement(prop, "DefaultValue")
        default_value.text = str(values[2])
        default_value.tail = "\n\t\t"
        
        
        audio_engine_prop_id = ET.SubElement(prop, "AudioEnginePropertyID")
        audio_engine_prop_id.text = str(audio_engine_prop_id_counter)
        audio_engine_prop_id_counter += 1
        audio_engine_prop_id.tail = "\n\t\t"
        
        restrictions = ET.SubElement(prop, "Restrictions")
        restrictions.text = "\n\t\t\t"
        value_restriction = ET.SubElement(restrictions, "ValueRestriction")
        value_restriction.text = "\n\t\t\t\t"
        value_range = ET.SubElement(value_restriction, "Range", Type="Real32")
        value_range.text = "\n\t\t\t\t\t"
        
        value_min = ET.SubElement(value_range, "Min")
        value_min.text = str(values[0])
        value_min.tail = "\n\t\t\t\t\t"
        
        value_max = ET.SubElement(value_range, "Max")
        value_max.text = str(values[1])
        value_max.tail = "\n\t\t\t\t"
        value_range.tail = "\n\t\t\t"
        value_restriction.tail = "\n\t\t"
        restrictions.tail = "\n\t"
        
        # Add tail for Property element
        prop.tail = "\n\n\t"
        
        xml_data = xml_header + ET.tostring(root, encoding='unicode', method='xml') + xml_footer
        
    return xml_data


def generate_library(file_name, output_folder):
    extensions = ['.dll', '.lib','.pbd','.exp']
    dir = os.path.dirname(os.path.realpath(__file__))
    content_folder = "lib"

    for ext in extensions:
        original_file = os.path.join(dir, content_folder, f"CsoundWwise{ext}")
        generated_file = os.path.join(output_folder, f"{file_name}{ext}")

        if os.path.exists(original_file):
            shutil.copy(original_file, generated_file)

        else:
            print(f"File '{original_file}' not found.")

def main():
    csd_file = input("Enter the path to the .csd file: ")
    properties = parse_csd_file(csd_file)
    xml_data = generate_xml(properties, csd_file)
    
    # Extract the file name without extension
    file_name_without_extension = csd_file.split('.')[0]
    output_folder = os.path.join(os.path.dirname(os.path.realpath(__file__)), file_name_without_extension)
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    xml_file = os.path.join(output_folder, f"{file_name_without_extension}.xml")

    print(f"{xml_file}")

    generate_library(file_name_without_extension, output_folder)

    with open(xml_file, 'w') as f:
        f.write(xml_data)

    print(f"Files has been written")


if __name__ == "__main__":
    main()
