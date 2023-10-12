# Les valeurs à inclure dans le fichier LTR
name = "One block into the water"
solver = "Yono-Yono"
score = 160
sequence = "RRUFFUULLUUURRDFDLLDDRFFFRRRDDDRRRUUULFDDDLLLUFFFUUUURRUUURRUUULLLDDLLDDDRFUUURRUURRRDDDLLDFDLLDDRFFFRRRDDRRRRRUUULLLLDFDLLDDRFFFRUUURRRRDFFDDDLLDDDRRRUULFFLLLL"

# Nom du fichier de sortie
fichier_ltr = "mon_fichier2.ltr"

# Ouvrir le fichier en mode écriture
with open(fichier_ltr, "w") as fichier:
    fichier.write(f"Name: {name}\n")
    fichier.write(f"Solver: {solver}\n")
    fichier.write(f"Score: {score}\n")
    fichier.write(f"{sequence}\n")

print(f"Le fichier {fichier_ltr} a été créé avec succès.")
