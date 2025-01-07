from disjoint_set import DisjointSet 
from typing import Generic, List, Set, Dict, TypeVar, Generator , Counter
from heapq import heappop, heappush
from dataclasses import dataclass, field

T = TypeVar('T')

def display_columns(columns: DisjointSet[T]):
    print("Disjoint Set Contents:")
    # Use the all_classes generator to get all equivalence classes
    for equivalence_class in columns.all_classes():
        print(f"Equivalence Class: {equivalence_class}")

# Usage:
columns = DisjointSet[int]()  # Example instantiation for integers
columns.new_class(6,7)
columns.add(1)
columns.add(2)
columns.add(3)
columns.add(4)
#print({columns.contains(1)})
columns.union(1, 2)
### they are in the same equivelence class 
print(f"==> (1,2) {columns.is_equivalent(1,2)}")
assert  (columns.contains(1) and columns.contains(3)), (1, 3)
columns.union(3, 4)
print(f"==> (3,4) {columns.is_equivalent(3,4)}")
display_columns(columns)
columns.add_to(11,1)
columns.add_to(12,2)
columns.add_to(13,3)
columns.add_to(14,4)
#print("==== Updated columns ====")
#display_columns(columns)
print("===== Counter Class usage ============")
counter = Counter(['a', 'b', 'a', 'c', 'b', 'b'])
print(counter)
print(counter.most_common(2))
counter.subtract(['b', 'c'])
print(counter)
# add more occurences using update method 
counter.update(['a', 'b', 'b', 'd'])
print(counter)
print("############ Test the priority queue #########")
# we use this decorator 
# to simpllify the creattion of classes that are primarly
# used to store data. it automatically generates special methods 
# such as __init__ , __repr__ , __eq__  
"""
When you specify order=True in a @dataclass, 
Python uses the attributes of the class in the order 
they are defined to determine the order of instances. 
This follows the "lexicographical order" rule

# to exclude a field from comparaison we use the field(compare=false)
Ex : 
    @dataclass(order=True)
    class schedule:
        cost: int | float
        name: str = field(compare=False)
"""
@dataclass(order=True)
class schedule:
    cost: int | float
    name : str 
First = schedule(cost=20, name="Moyen")
Second1 = schedule(cost=10, name="Bobi")
Second2 = schedule(cost=10, name="Aobe")
Third = schedule(cost=30, name="Soble")
pqueue: List[schedule] = []
heappush(pqueue, First)
heappush(pqueue, Second1)
heappush(pqueue, Second2)
heappush(pqueue, Third)
cur = heappop(pqueue) 
print(cur.cost,cur.name) # (10,Aobe)
cur = heappop(pqueue) 
print(cur.cost,cur.name) # (10,Bobi)
#print(First.cost,First.name)
