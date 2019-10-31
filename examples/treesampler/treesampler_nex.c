#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "avl.h"


/*
#define	 DEBUG 1
*/

struct	 avl_table * index2NodePointer = NULL;
long	 totalNodesMade = 0;

/*---------------------------------------------------------------------------------------------------- */

void 		init_genrand	(unsigned long);
unsigned long 	genrand_int32	(void);

/*---------------------------------------------------------------------------------------------------- */
struct   tn {

    struct tn *left, 
              *right, 
              *parent;
              
    double    d_root,
              d_branch;


    struct avl_table 
              * recipients;
              
    char      visited;
    long      node_index;
};

/*---------------------------------------------------------------------------------------------------- */

struct tn *   treeRoot,
          **  eligible_nodes = NULL;
        
char	  *   can_use	     = NULL;

long	      eligible_node_count = 0;

/*---------------------------------------------------------------------------------------------------- */

struct   avl_entry {
    struct tn * tree_node;
    double time_from_source;
};

/*---------------------------------------------------------------------------------------------------- */

struct   avl_map_entry {
    struct tn * tree_node;
    long   node_index;
};

/*---------------------------------------------------------------------------------------------------- */

int compare_times (const void *avl_a, const void *avl_b, void * avl_param)
{
    double t1 = ((struct avl_entry*)avl_a)->time_from_source;
    double t2 = ((struct avl_entry*)avl_b)->time_from_source;
    
    if (t1 > t2) return 1;
    if (t1 < t2) return -1;
    return 0;
}

/*---------------------------------------------------------------------------------------------------- */

int compare_nodes (const void *avl_a, const void *avl_b, void * avl_param)
{
    long n1 = ((struct avl_map_entry*)avl_a)->node_index;
    long n2 = ((struct avl_map_entry*)avl_b)->node_index;
    
    if (n1 > n2) return 1;
    if (n1 < n2) return -1;
    return 0;
}

/*---------------------------------------------------------------------------------------------------- */

void	count_eligible_nodes (struct tn* a_node, double time_slice, char must_be_alive)
{
    if (a_node->left)
        count_eligible_nodes (a_node->left, time_slice, must_be_alive);
    if (a_node->right)
        count_eligible_nodes (a_node->right, time_slice, must_be_alive);
        
   if (a_node->left == NULL && a_node->right == NULL && (a_node->d_root >= time_slice || a_node->d_root < 0 || must_be_alive == 0))
    {
        a_node->visited = 1;
        eligible_node_count ++;
    }
    else
        a_node->visited = 0;
}

/*---------------------------------------------------------------------------------------------------- */

void	map_eligible_nodes (struct tn* a_node, long * index)
{
    if (a_node->visited)
    {
        eligible_nodes[(*index)++] = a_node;
        a_node->visited = 0;
    }
    if (a_node->left)
        map_eligible_nodes (a_node->left, index);
    if (a_node->right)
        map_eligible_nodes (a_node->right, index);
}

/*---------------------------------------------------------------------------------------------------- */

void	check_pointer (void * p)
{
    if (p == NULL)
    {
        fprintf (stderr,"Memory allocation error\n");
        exit (1);
    }

}
/*---------------------------------------------------------------------------------------------------- */

struct tn * make_new_node (const long node_index)
{
    struct tn * nn = (struct tn*)malloc(sizeof (struct tn));
    struct avl_map_entry * ave = (struct avl_map_entry*)malloc(sizeof (struct avl_map_entry));
    check_pointer (nn);
    check_pointer (ave);
    nn->left = nn->right = nn->parent = NULL;
    nn->d_root = nn->d_branch = 0.0;
    nn->recipients = NULL;
    nn->visited    = 0;
    nn->node_index = node_index;
    if (node_index >= 0)
    {
        ave->tree_node = nn;
        ave->node_index = node_index;
        check_pointer (avl_probe (index2NodePointer, ave));
    }
    else
        free (ave);
        
    /*#ifdef DEBUG
            printf ("\nMade node index %ld", node_index);
    #endif*/
    
    totalNodesMade++;
    return nn;
}

/*---------------------------------------------------------------------------------------------------- */

long validate_the_tree (struct tn * cNode, const long sourceID, double timePoint)
{
    long n1,
         n2;
         
    double t_stamp = 0.0;
         
    if (cNode)
    {
        #ifdef DEBUG
            printf ("\nNode %ld. Left Child: ", cNode->node_index);
            if (cNode->left)
                printf ("%ld",cNode->left->node_index);
            else
               printf ("None");
            printf (". Right Child: ");
            if (cNode->right)
                printf ("%ld",cNode->right->node_index);
            else
               printf ("None");

            printf (". Parent: ");
            if (cNode->parent)
                printf ("%ld",cNode->parent->node_index);
            else
               printf ("None");
        #endif
        if (cNode->visited < 0)
        {
            fprintf (stderr,"Cycles found in the tree structure: individuals %ld and %ld form a circular infection path\n", cNode->node_index, sourceID);
            return -1;
        }
        if (cNode->parent) 
        {
            if (cNode->d_root > 0)
                t_stamp =  cNode->d_root - cNode->parent->d_root;
            else
                 t_stamp =  timePoint - cNode->parent->d_root;
        }
        else
            t_stamp = cNode->d_root;
        if (t_stamp < 0.0)
        {
            fprintf (stderr,"Anachronisms found between individuals %ld and %ld\n", cNode->node_index, sourceID);
            return -1;
        }
        cNode->d_branch = t_stamp;
        if ((n1=validate_the_tree (cNode->left,cNode->node_index, timePoint))<0) return -1;
        if ((n2=validate_the_tree (cNode->right,cNode->node_index, timePoint))<0) return -1;
        cNode->visited = -1;
        return n1+n2+1;
    }
    return 0;
}

/*---------------------------------------------------------------------------------------------------- */

void print_the_tree (struct tn * tree_root)
{
    if (tree_root->left || tree_root->right)
    /* not a leaf */
    {
        printf ("(");
        if (tree_root->left)
            print_the_tree (tree_root->left);
        if (tree_root->right)
        {
            if (tree_root->left)
                printf (",");
           print_the_tree (tree_root->right);
        }
        printf (")");
        if (tree_root->parent)
            printf (":%g",tree_root->d_branch);
            
    }
    else
        printf ("%ld:%g",tree_root->node_index, tree_root->d_branch);
}

/*---------------------------------------------------------------------------------------------------- */

void process_reduced_tree (struct tn* tree_root)
{
    if (tree_root->left || tree_root->right)
    /* not a leaf */
    {
        tree_root->visited = 0;
        if (tree_root->left)
            process_reduced_tree (tree_root->left);
        if (tree_root->right)
           process_reduced_tree (tree_root->right);
         if (tree_root->parent == NULL || tree_root->left && tree_root->left->visited || tree_root->right  && tree_root->right->visited)
            tree_root->visited = 1;
    }
}

/*---------------------------------------------------------------------------------------------------- */

void print_reduced_tree (struct tn * tree_root, double add_factor)
{
    if (tree_root->left || tree_root->right)
    /* not a leaf */
    {
        if (tree_root->left && tree_root->left->visited && tree_root->right  && tree_root->right->visited || tree_root->parent == NULL)
        {
            printf ("(");
            if (tree_root->left)
                print_reduced_tree (tree_root->left,0.0);
            if (tree_root->right)
            {
                if (tree_root->left)
                    printf (",");
                print_reduced_tree (tree_root->right,0.0);
            }
            printf (")");
            if (tree_root->parent)
                printf (":%g",tree_root->d_branch+add_factor);
        }
        else
        {
            if (tree_root->left)
                print_reduced_tree (tree_root->left,add_factor+tree_root->d_branch);
            if (tree_root->right)
                print_reduced_tree (tree_root->right,add_factor+tree_root->d_branch);
        }            
    }
    else
        if (tree_root->visited)
        {
            printf ("%ld:%g",tree_root->node_index, add_factor+tree_root->d_branch);
            tree_root->visited = 0;
        }
}


/*---------------------------------------------------------------------------------------------------- */

void make_default_chain	 	(struct tn * parent)
{
     struct avl_entry 		*prev_entry = (struct avl_entry*)malloc (sizeof (struct avl_entry));
     struct tn			*chain_stopper;
     parent->recipients = avl_create (compare_times, NULL, NULL);
     prev_entry = (struct avl_entry*)malloc (sizeof (struct avl_entry));
     chain_stopper = make_new_node (-1);
     check_pointer (prev_entry);
     check_pointer (chain_stopper);
     
     chain_stopper->d_root = -1.0;
     chain_stopper->parent = parent;
     chain_stopper->node_index = parent->node_index;
     prev_entry->tree_node = chain_stopper;
     prev_entry->time_from_source = 1e300; 
     chain_stopper->parent = parent; 
     if (parent->visited) /* right */
         parent->right = chain_stopper;
     else
         parent->left = chain_stopper;
         
     if (!avl_probe (parent->recipients, prev_entry))
     {
         fprintf (stderr,"AVL Instertion Error\n");
         exit (1);
     }
}

/*---------------------------------------------------------------------------------------------------- */

void check_chain_compatibility (struct tn * top_node, int orientation)
{
    struct avl_entry	*the_chain;
    struct avl_traverser at;
    struct tn		*swap;
    if (top_node->visited != orientation && top_node->recipients)
    {
       avl_t_init (&at, top_node->recipients);
       the_chain = avl_t_first (&at,top_node->recipients);
       while (the_chain)
       {
            check_chain_compatibility (the_chain->tree_node, !orientation);
            the_chain = avl_t_next (&at);
       }	
       swap = top_node->left;
       top_node->left = top_node->right;
       top_node->right = swap;
       top_node->visited = orientation;
    }
}

/*---------------------------------------------------------------------------------------------------- */

char insert_a_child (struct tn * parent, struct tn * child)
{
    struct avl_entry 		*new_entry = (struct avl_entry*)malloc (sizeof (struct avl_entry)),
                                *prev_entry,
                                *next_entry;
    
    struct avl_traverser 	at;
    
    check_pointer (new_entry);
       
    new_entry->tree_node        = child;
    new_entry->time_from_source = child->d_root;
    
    if (parent->recipients == NULL)
    /* create default transmission sequence / insert self at the very end */
        make_default_chain (parent);

   /* #ifdef DEBUG
    printf ("\nEntry: %ld, %ld, %g", parent->node_index, child->node_index, child->d_root);
    #endif */
    
    if (child->node_index == -1) /* node expired */
    {
        new_entry->time_from_source = 1e300;
        prev_entry = avl_find (parent->recipients,new_entry);
        if (prev_entry == NULL)
        {
            fprintf (stderr,"Termination Branch Instertion Error - 2 termination events for node %ld\n",parent->node_index);
            return 0;
        }
        prev_entry->tree_node->d_root = child->d_root;
        free (child);
        free (new_entry);
        totalNodesMade--;
   }
   else
   {
        if (!avl_probe (parent->recipients, new_entry))
        {
            fprintf (stderr,"AVL Instertion Error\n");
            return 0;
        }
        
        avl_t_init (&at, parent->recipients);
        prev_entry = avl_t_find (&at,parent->recipients,new_entry);
        prev_entry = avl_t_prev (&at);
        next_entry = avl_t_find (&at,parent->recipients,new_entry);
        next_entry = avl_t_next (&at);
        
        if (!next_entry)
        {
            fprintf (stderr,"Internal infection chain error\n");
            return 0;
        }
        
        /* next_entry will always be present -> the terminator of the actual node source */
        if (prev_entry)
        {
             child->parent = prev_entry->tree_node;
            if (parent->visited) /* right chain */
            {
                prev_entry->tree_node->right = child;
                child->right = next_entry->tree_node;
            }
            else
            {
                prev_entry->tree_node->left = child;
                child->left = next_entry->tree_node;
            }
        }
        else
        {
            child->parent = parent;
            if (parent->visited) /* right chain */
            {
                parent->right = child;
                child->right = next_entry->tree_node;
            }
            else
            {
                parent->left = child;                
                child->left = next_entry->tree_node;
            }
        }
        next_entry->tree_node->parent = child;
        if (!child->recipients)
        {
           child->visited = !parent->visited;
           make_default_chain(child);
        }
        else
            check_chain_compatibility (child,!parent->visited);
    }
   
    return 1;
}

/*---------------------------------------------------------------------------------------------------- */

char  handleInputLine (const long source, const long recipient, const double timePoint, const long line)
{
    struct tn * nn_p,
              * nn_c;
   
    struct    avl_map_entry   searchStruct = {NULL, source};
    
    void      *e1 = avl_find (index2NodePointer, &searchStruct),
              *e2 = NULL;
              
    searchStruct.node_index = recipient;
    e2 = avl_find (index2NodePointer, &searchStruct);
    
    if (e1 && e2)
    {
        fprintf (stderr,"Duplicate transmission event on line %ld\n",line);
        return 0;
    }
    else
    {
        if (e1 == NULL) /* create parent node */
            nn_p = make_new_node (source);
        else
            nn_p = ((struct    avl_map_entry*)e1)->tree_node;
            
        if (e2 == NULL)
            nn_c = make_new_node (recipient);
        else
            nn_c = ((struct    avl_map_entry*)e2)->tree_node;
            
        /*nn_c->parent 	= nn_p;*/
        nn_c->d_root 	= timePoint;
        if (!insert_a_child      (nn_p, nn_c))
            return 0;
     }
     
     if (!treeRoot || nn_c == treeRoot)
        treeRoot = nn_p;
         
    return 1;
}

/*---------------------------------------------------------------------------------------------------- */

int main (int argc, const char * argv[]) 
{
    FILE  *inFile;
    
    double timePoint,
           eventTime;
           
    long   dummy,
           source,
           recipient,
           lineNumber = 1,
           sample_size = 0,
           sample_count = 1;
           
    char   retCode = 0,
           db [512],
           force_alive = 0;
           
    time_t current_time;
    
    if (argc < 3 || argc > 6)
    {
        fprintf (stderr,"Incorrect number of arguments.\nExpected arguments: filename timepoint [sample_size sample_count force_alive]\n");
        return 1;
    }
    inFile    = fopen (argv[1], "r"); 
    
    if (!inFile)
    {
        fprintf (stderr,"Failed to open input file: %s\n", argv[1]);
        return 1; 
    }
    
    index2NodePointer = avl_create (compare_nodes,NULL,NULL);
    
    timePoint = atof (argv[2]);
    if (timePoint <= 0.0)
       timePoint = 1e300; 
       
    if (argc > 3)
    {
        sample_size = atoi (argv[3]);
        if (argc > 4)
        {
            sample_count = atoi (argv[4]);
            if (sample_count < 1)
                sample_count = 1;
            if (argc > 5)
                force_alive = atoi (argv[5]);
        }
    }
       
    /* skip top line */
    
    dummy = fscanf (inFile, "%511s %511s %511s %511s", db, db, db, db);
    
    while (!feof (inFile))
    {
        lineNumber ++;
        if ((dummy = fscanf (inFile, "%ld %ld %ld %lg ", &dummy, &source, &recipient,&eventTime)) != 4)
        {
              fprintf (stderr,"Incorrect number of arguments (%ld) on line %ld\n", dummy, lineNumber);     
              retCode = 1;
              break; 
        }
        if (source < 0 || eventTime < 0.0)
        {
            fprintf (stderr,"Invalid source code or time stamp in line %ld\n", lineNumber);
            retCode = 1;
            break;
        }
        if (eventTime <= timePoint)
            if (handleInputLine (source, recipient, eventTime, lineNumber) == 0)
            {
                retCode = 1;
                break;
            }
    }
    fclose (inFile);
    if (retCode == 0)
    {
        dummy = validate_the_tree (treeRoot,-1,timePoint);
        if (dummy > 0)
        {
            if (dummy != totalNodesMade)
            {
                fprintf (stderr,"There appear to be disconnected components in the infection hierarchy\n");
                retCode = 1;
            }
            else
            {
                if (sample_size < 1)
                {
                    printf ("BEGIN TREES;\nTREE complete_tree = ");
                    print_the_tree (treeRoot);
                    printf (";\nEND;\n");
                }
                else
                {
                    /* count eligible nodes */
                    count_eligible_nodes (treeRoot, timePoint, force_alive);
                    if (eligible_node_count <= sample_size)
                    {
                        printf ("BEGIN TREES;\nTREE complete_tree = ");
                        print_the_tree (treeRoot);
                        printf (";\nEND;\n");
                    }
                    else
                    {
                        printf ("BEGIN TREES;");
                        eligible_nodes = (struct tn**)malloc (eligible_node_count*sizeof (struct tn*));
                        check_pointer (eligible_nodes);
                        dummy = 0;
                        map_eligible_nodes (treeRoot,&dummy);
                        can_use        = (char*)malloc (eligible_node_count*sizeof (char));
                        check_pointer (can_use);
                        time (&current_time);
                        init_genrand (current_time);

                        for (dummy = 0; dummy < sample_count; dummy++)
                        {
                            for (source = 0; source < eligible_node_count; source ++)
                                can_use[source] = 1;
                                
                            for (source = 0; source < sample_size; source ++)
                            {
                                recipient = genrand_int32() % eligible_node_count;
                                while (can_use[recipient] == 0)	
                                    recipient = genrand_int32() % eligible_node_count;
                                can_use[recipient] = 0;
                                eligible_nodes[recipient]->visited = 1;
                            }
                            /*for (source = 0; source < eligible_node_count; source++)
                                if (can_use[source] == 0)
                                    printf ("%ld (%ld) ", source, eligible_nodes[source]->node_index);
                            printf ("\n");*/
                            process_reduced_tree (treeRoot);
                            printf ("\tTree sample_tree_%ld = ", dummy);
                            print_reduced_tree (treeRoot,0.0);
                            printf (";\n");
                       }
                       printf ("END;");
                    }
                }
            }
        }
    }
    return retCode;
}
