string="a*"
case "$string" in
    "a\*")
        echo "Matched 'a\*'"
        ;;
    "a*")
        echo "Matched 'b*'"
        ;;
    *)
        echo "No match"
        ;;
esac
