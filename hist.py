def get_history(dumps):
    """
    Prints the changes between consecutive lists in the 'dumps' list,
    considering the position of each element.

    Args:
        dumps (list): A list of lists representing the evolution of a data structure over time.
    """
    for i in range(len(dumps) - 1):
        print(f"Changes from dumps[{i}] to dumps[{i + 1}]:")

        # Get the elements that were added
        added = [
            dumps[i + 1][j]
            for j in range(len(dumps[i + 1]))
            if j >= len(dumps[i]) or dumps[i + 1][j] != dumps[i][j]
        ]
        if added:
            print("Elements added:", added)

        # Get the elements that were removed
        removed = [
            dumps[i][j]
            for j in range(len(dumps[i]))
            if j >= len(dumps[i + 1]) or dumps[i][j] != dumps[i + 1][j]
        ]
        if removed:
            print("Elements removed:", removed)

        # Get the elements that stayed the same and their positions
        unchanged = [
            (j, dumps[i][j])
            for j in range(min(len(dumps[i]), len(dumps[i + 1])))
            if dumps[i][j] == dumps[i + 1][j]
        ]
        if unchanged:
            print("Elements unchanged:", unchanged)

        print()
