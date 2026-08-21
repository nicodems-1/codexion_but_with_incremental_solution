#!/bin/bash

# ==============================================================================
# CONFIGURATION
# ==============================================================================
PROG="./codexion"
TIMEOUT_CMD="timeout 10s" # Coupe le programme si deadlock (bloqué > 10s)

# Couleurs pour faire joli
GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
YELLOW="\033[0;33m"
RESET="\033[0m"

# ==============================================================================
# FONCTION DE TEST
# ==============================================================================
# Arguments: $1=Nom du test, $2=Résultat attendu (survive/die/error), $3...=Args
run_test() {
    TEST_NAME=$1
    EXPECTED=$2
    shift 2
    ARGS="$@"

    echo -e "${BLUE}▶ TEST : ${TEST_NAME}${RESET}"
    echo -e "  Commande : ${PROG} ${ARGS}"
    
    # Lancement avec timeout
    OUTPUT=$($TIMEOUT_CMD ${PROG} ${ARGS} 2>&1)
    EXIT_CODE=$?

    # Analyse de la sortie
    HAS_BURNOUT=$(echo "$OUTPUT" | grep -i "burnout")
    
    if [ $EXIT_CODE -eq 124 ]; then
        echo -e "  ${RED}❌ ECHEC : DEADLOCK DETECTE (Timeout dépassé)${RESET}\n"
        return
    fi

    if [ "$EXPECTED" == "error" ]; then
        if [ $EXIT_CODE -ne 0 ] || [[ "$OUTPUT" == *"Error"* ]]; then
            echo -e "  ${GREEN}✅ SUCCES : Erreur bien gérée${RESET}\n"
        else
            echo -e "  ${RED}❌ ECHEC : Le programme aurait dû renvoyer une erreur${RESET}\n"
        fi
    elif [ "$EXPECTED" == "survive" ]; then
        if [ -z "$HAS_BURNOUT" ]; then
            echo -e "  ${GREEN}✅ SUCCES : Tout le monde a survécu (Fin propre)${RESET}\n"
        else
            echo -e "  ${RED}❌ ECHEC : Quelqu'un a fait un burnout inattendu${RESET}\n"
        fi
    elif [ "$EXPECTED" == "die" ]; then
        if [ ! -z "$HAS_BURNOUT" ]; then
            echo -e "  ${GREEN}✅ SUCCES : Burnout confirmé au bon moment${RESET}\n"
        else
            echo -e "  ${RED}❌ ECHEC : Les codeurs auraient dû mourir !${RESET}\n"
        fi
    fi
}

echo -e "${YELLOW}==============================================="
echo -e "🚀 LANCEMENT DE LA BATTERIE DE TESTS CODEXION"
echo -e "===============================================${RESET}\n"

# ==============================================================================
# 1. TESTS DE PARSING (Gestion des erreurs)
# ==============================================================================
echo -e "${YELLOW}--- 1. TESTS DE PARSING ---${RESET}"
run_test "Arguments manquants" "error" 5 800 200 200 200
run_test "Argument négatif" "error" -5 800 200 200 200 5 0 fifo
run_test "Mauvais type (lettres)" "error" 5 abc 200 200 200 5 0 fifo
run_test "Scheduler invalide" "error" 5 800 200 200 200 5 0 random

# ==============================================================================
# 2. SURVIE DE BASE (Les conditions sont larges)
# ==============================================================================
echo -e "\n${YELLOW}--- 2. TESTS DE SURVIE CLASSIQUES (5 Compilations) ---${RESET}"
run_test "Pair - FIFO (Temps larges)" "survive" 4 800 200 200 200 5 0 fifo
run_test "Impair - FIFO (Temps larges)" "survive" 5 800 200 200 200 5 0 fifo
run_test "Pair - EDF (Temps larges)" "survive" 4 800 200 200 200 5 0 edf
run_test "Impair - EDF (Temps larges)" "survive" 5 800 200 200 200 5 0 edf

# ==============================================================================
# 3. MORT INÉVITABLE
# ==============================================================================
echo -e "\n${YELLOW}--- 3. TESTS DE BURNOUT INEVITABLE ---${RESET}"
# Le cycle (compile+debug+refactor) prend 600ms, mais le burnout est à 300
run_test "Burnout évident (Trop lent) FIFO" "die" 4 300 200 200 200 5 0 fifo
run_test "Burnout évident (Trop lent) EDF" "die" 4 300 200 200 200 5 0 edf
# 1 seul codeur (ne peut pas prendre 2 dongles)
run_test "Le codeur solitaire (Doit mourir à 300ms)" "die" 1 300 200 200 200 5 0 fifo

# ==============================================================================
# 4. LE FIL DU RASOIR (Stress temporel)
# ==============================================================================
echo -e "\n${YELLOW}--- 4. LIMITES ET STRESS (Fil du rasoir) ---${RESET}"
# Burnout = 600, Cycle = 600 (Devrait mourir avec les lags CPU)
run_test "Fil du rasoir (Mort attendue à cause du lag)" "die" 4 600 200 200 200 5 0 fifo
# Burnout = 610, Cycle = 600 (Devrait survivre de justesse)
run_test "Fil du rasoir (+10ms de marge de survie)" "survive" 4 610 200 200 200 5 0 edf

# ==============================================================================
# 5. CHARGE LOURDE (Beaucoup de threads)
# ==============================================================================
echo -e "\n${YELLOW}--- 5. CHARGE LOURDE (Risque de Deadlock) ---${RESET}"
run_test "50 Codeurs (FIFO) - 2 compilations" "survive" 50 1500 200 200 200 2 0 fifo
run_test "50 Codeurs (EDF) - 2 compilations" "survive" 50 1500 200 200 200 2 0 edf

echo -e "\n${YELLOW}=== FIN DES TESTS ===${RESET}"