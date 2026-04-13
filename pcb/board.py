"""
Hitscan vest controller — netlist generator (no deps).
Edit COMPONENTS / NETS below, run `python3 board.py` → regenerates hitscan.net.
Import into KiCad PCB editor: File → Import → Netlist.
"""

COMPONENTS = [
    ("J4", "ESP32_left",  "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical"),
    ("J5", "ESP32_right", "Connector_PinSocket_2.54mm:PinSocket_1x15_P2.54mm_Vertical"),
    ("J1", "PAD_CHEST",   "TerminalBlock:TerminalBlock_bornier-2_P5.08mm"),
    ("J2", "PAD_BACK",    "TerminalBlock:TerminalBlock_bornier-2_P5.08mm"),
    ("J3", "RING_LED",    "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical"),
    ("R1", "22R 0.5W",    "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P12.70mm_Horizontal"),
    ("R2", "1R",          "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal"),
    ("BZ1","active_buzzer","Buzzer_Beeper:Buzzer_12x9.5RM7.6"),
    ("SW1","reset",       "Button_Switch_THT:SW_PUSH_6mm"),
    ("H1", "M3", "MountingHole:MountingHole_3.2mm_M3"),
    ("H2", "M3", "MountingHole:MountingHole_3.2mm_M3"),
    ("H3", "M3", "MountingHole:MountingHole_3.2mm_M3"),
    ("H4", "M3", "MountingHole:MountingHole_3.2mm_M3"),
]

# ESP32 DevKit V1 30-pin: J4=left, J5=right, pin1=antenna end.
# J4: 6=GPIO32 14=GND 15=VIN | J5: 6=GPIO19 7=GPIO18 8=GPIO5 14=GND 15=3V3
NETS = {
    "3V3":    [("J5",15), ("R1",1)],
    "PAD_HI": [("R1",2),  ("J1",1), ("J2",1)],
    "P32":    [("J4",6),  ("R2",1), ("J1",2), ("J2",2)],
    "GND":    [("J4",14), ("J5",14),("R2",2), ("BZ1",2),("SW1",2),("J3",2)],
    "VIN":    [("J4",15), ("J3",1)],
    "P5":     [("J5",8),  ("J3",3)],
    "P18":    [("J5",7),  ("BZ1",1)],
    "P19":    [("J5",6),  ("SW1",1)],
}

def emit():
    out = ['(export (version "E")', '  (design (source "board.py") (tool "hitscan-gen"))',
           '  (components']
    for ref, val, fp in COMPONENTS:
        out.append(f'    (comp (ref "{ref}") (value "{val}")')
        out.append(f'      (footprint "{fp}"))')
    out.append('  )')
    out.append('  (nets')
    for i, (name, nodes) in enumerate(NETS.items(), 1):
        out.append(f'    (net (code "{i}") (name "{name}")')
        for ref, pin in nodes:
            out.append(f'      (node (ref "{ref}") (pin "{pin}"))')
        out.append('    )')
    out.append('  ))')
    return "\n".join(out) + "\n"

if __name__ == "__main__":
    with open("hitscan.net", "w") as f:
        f.write(emit())
    print(f"wrote hitscan.net — {len(COMPONENTS)} parts, {len(NETS)} nets")
