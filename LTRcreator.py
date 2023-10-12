def create_ltr_file(name, solver, sequence, output_file):
    # Calculer la longueur de la séquence pour le score
    score = len(sequence)

    # Ouvrir le fichier en mode écriture
    with open(output_file, "w") as fichier:
        fichier.write(f"Name: {name}\n")
        fichier.write(f"Solver: {solver}\n")
        fichier.write(f"Score: {score}\n")
        fichier.write(f"\n{sequence}\n")

    print(f"Le fichier {output_file} a été créé avec succès.")

# Utilisation de la fonction avec des valeurs spécifiques
name = "One block into the water"
solver = "Yono-Yono"
sequence = "RRUFFUULLUUURRDFDLLDDRFFFRRRDDDRRRUUULFDDDLLLUFFFUUUURRUUURRUUULLLDDLLDDDRFUUURRUURRRDDDLLDFDLLDDRFFFRRRDDRRRRRUUULLLLDFDLLDDRFFFRUUURRRRDFFDDDLLDDDRRRUULFFLLLL"
output_file = "mon_fichier2.ltr"

create_ltr_file(name, solver, sequence, output_file)
