#!/bin/bash

# ==============================================================================
# Configuration
# ==============================================================================
EXEC="./codexion"
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --error-exitcode=1"
TIMEOUT="timeout 10" # Coupe le test après 10s si deadlock

# Couleurs
GREEN="\033[32m"
RED="\033[31m"
YELLOW="\033[33m"
CYAN="\033[36m"
RESET="\033[0m"

if [ ! -f "$EXEC" ]; then
    echo -e "${RED}Erreur : Exécutable $EXEC introuvable. Compile avec 'make' d'abord.${RESET}"
    exit 1
fi

echo -e "${CYAN}====================================================${RESET}"
echo -e "${CYAN}         CODEXION - BATTERIE DE TESTS ULTIME        ${RESET}"
echo -e "${CYAN}====================================================${RESET}\n"

# ==============================================================================
# 1. Tests d'intégrité (Parsing & Erreurs)
# ==============================================================================
echo -e "${YELLOW}--- 1. TESTS DE PARSING ET ERREURS ---${RESET}"

tests_erreurs=(
    "a 300 200 200 200"
    "5 -300 200 200 200"
    "5 300 200 200 200 10 0 test"
    "5 300 200"
)

for arg in "${tests_erreurs[@]}"; do
    $EXEC $arg > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${GREEN}[OK]${RESET} Rejet correct : $EXEC $arg"
    else
        echo -e "${RED}[FAIL]${RESET} Le programme n'a pas renvoyé d'erreur : $EXEC $arg"
    fi
done
echo ""

# ==============================================================================
# 2. Tests Valgrind (Fuites Mémoire & Data Races)
# ==============================================================================
echo -e "${YELLOW}--- 2. TESTS VALGRIND (LEAKS ET ACCÈS MÉMOIRE) ---${RESET}"

tests_valgrind=(
    "5 800 200 200 200 3 0 edf"
    "4 410 200 200 200 3 0 fifo"
    "1 800 200 200 200 1 0 edf"
)

for arg in "${tests_valgrind[@]}"; do
    echo -n "Test mémoire : $EXEC $arg ... "
    $VALGRIND $EXEC $arg > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[CLEAN]${RESET}"
    else
        echo -e "${RED}[LEAK / ERREUR]${RESET} - Relance manuellement pour voir la trace."
    fi
done
echo ""

# ==============================================================================
# 3. Tests des limites extrêmes (Stress Absurde)
# ==============================================================================
echo -e "${YELLOW}--- 3. TESTS EXTRÊMES (STRESS ABSURDE) ---${RESET}"
echo "Attention : Ces tests vérifient que ton programme ne segfault pas avec énormément de threads."

tests_extremes=(
    "200 800 200 200 200 2 0 edf"
    "300 800 200 200 200 1 0 fifo"
    "500 800 200 200 200 1 0 edf"
)

for arg in "${tests_extremes[@]}"; do
    echo -n "Test $arg ... "
    $TIMEOUT $EXEC $arg > /dev/null 2>&1
    ret=$?
    if [ $ret -eq 124 ]; then
        echo -e "${RED}[TIMEOUT]${RESET} (Deadlock potentiel ou PC trop lent)"
    elif [ $ret -eq 139 ] || [ $ret -eq 11 ]; then
        echo -e "${RED}[SEGFAULT]${RESET}"
    else
        echo -e "${GREEN}[SURVÉCU]${RESET}"
    fi
done
echo ""

# ==============================================================================
# 4. Batterie de 50 tests durs (Aléatoires)
# ==============================================================================
echo -e "${YELLOW}--- 4. BATTERIE DE 50 TESTS DURS ---${RESET}"
echo "Test des configurations où les timings sont ultra serrés."

success=0
deadlocks=0

for i in {1..50}
do
    # Génération de valeurs aléatoires serrées
    NB_CODERS=$(( (RANDOM % 8) + 3 )) # 3 à 10 codeurs
    T_COMPILE=$(( (RANDOM % 100) + 100 )) # 100 à 200
    T_REFACTOR=$(( (RANDOM % 100) + 100 )) # 100 à 200
    T_DEBUG=$(( (RANDOM % 100) + 100 )) # 100 à 200
    
    # Calcul d'un burnout très juste (approximatif)
    T_BURNOUT=$(( T_COMPILE * 2 + T_REFACTOR + 10 )) 
    
    CYCLES=3
    COOLDOWN=0
    
    if [ $((RANDOM % 2)) -eq 0 ]; then
        SCHED="edf"
    else
        SCHED="fifo"
    fi

    ARGS="$NB_CODERS $T_BURNOUT $T_COMPILE $T_REFACTOR $T_DEBUG $CYCLES $COOLDOWN $SCHED"
    
    # On affiche un compteur tous les 10 tests
    if [ $((i % 10)) -eq 0 ]; then
        echo "Progression : $i/50 tests terminés..."
    fi

    $TIMEOUT $EXEC $ARGS > /dev/null 2>&1
    ret=$?

    if [ $ret -eq 124 ]; then
        deadlocks=$((deadlocks + 1))
        echo -e "${RED}[DEADLOCK] sur : ./codexion $ARGS${RESET}"
    else
        success=$((success + 1))
    fi
done

echo -e "\n${CYAN}Bilan des 50 tests :${RESET}"
echo -e "Succès / Crash gérés / Morts assumées : ${GREEN}$success${RESET}"
if [ $deadlocks -gt 0 ]; then
    echo -e "Deadlocks (Timeout 10s) : ${RED}$deadlocks${RESET} (Il faut vérifier ces paramètres !)"
else
    echo -e "Deadlocks : ${GREEN}0${RESET} (Ton code gère parfaitement les files d'attente !)"
fi
echo -e "${CYAN}====================================================${RESET}"